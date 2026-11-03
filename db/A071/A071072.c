/*
 * A071072  Minimal "multiples of 4" set in base 10.
 * https://oeis.org/A071072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: full, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A071072: Minimal “multiples of 4” set in base 10  
   Algorithm:  
   • Keep a growing list of the terms already chosen.  
   • Enumerate successive multiples of 4 (0, 4, 8, 12, … in that order).  
   • For each multiple, convert it to a decimal string, and test whether
   any of the string representations of the already chosen terms occurs
   as a contiguous substring.  
   • If no chosen term is a substring, the multiple itself is appended
   to the list.  
   • The n‑th element of the list (1‑based) is a(n).  The algorithm
   uses only integer arithmetic and standard library routines,
   complying with the restrictions on seq_t and idx_t. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

/* convert a non‑negative unsigned long long to a decimal string.  
   caller supplies a buffer of size at least 21 bytes. */
static void ull_to_str(unsigned long long v, char *buf)
{
    if (v == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    char tmp[21];
    int pos = 0;
    while (v > 0) {
        unsigned int digit = v % 10;
        tmp[pos++] = '0' + digit;
        v /= 10;
    }
    int len = pos;
    for (int i = 0; i < len; ++i)
        buf[i] = tmp[len - 1 - i];
    buf[len] = '\0';
}

seq_t a(idx_t n)
{
    /* static state persisted between calls */
    static seq_t *terms   = NULL;   /* numeric values of chosen terms */
    static char **tstr    = NULL;   /* string representations of terms   */
    static size_t terms_len = 0;    /* current number of terms stored    */
    static size_t terms_cap = 0;    /* allocated capacity                */
    static unsigned long long next_mul = 0;  /* next multiple of 4 to examine */
    static idx_t computed   = 0;                    /* how many terms we have        */

    /* offset is 1; for n==0 we simply return 0 (unused) */
    if (n == 0) return (seq_t)0;

    /* grow the arrays if necessary */
    if (terms_cap == 0) {
        terms_cap = 16;
        terms   = mem_new(seq_t, terms_cap);
        tstr    = (char**)malloc(terms_cap * sizeof(char*));
    }

    /* generate terms until we have at least n of them */
    while (computed < n) {
        /* convert current multiple to a string */
        char numbuf[32];
        ull_to_str(next_mul, numbuf);

        /* test whether any existing term is a substring of this number */
        bool covered = false;
        for (size_t i = 0; i < terms_len; ++i) {
            if (strstr(numbuf, tstr[i]) != NULL) {
                covered = true;
                break;
            }
        }

        if (!covered) {
            /* store the new term: numeric and string */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms   = mem_resize(seq_t, terms, terms_cap);
                tstr    = (char**)realloc(tstr,   terms_cap * sizeof(char*));
            }
            terms[terms_len] = (seq_t)next_mul;

            /* duplicate the string representation */
            size_t len = strlen(numbuf) + 1;
            tstr[terms_len] = (char*)malloc(len);
            memcpy(tstr[terms_len], numbuf, len);

            ++terms_len;
            ++computed;
        }

        next_mul += 4ULL;  /* next multiple of 4 */
    }

    return terms[n - 1];
}
