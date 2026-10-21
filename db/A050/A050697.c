/*
 * A050697  Numbers that appear in A035140 but not in A050694.
 * https://oeis.org/A050697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   digit_counts, factor_u64, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050697: Numbers that belong to A035140 but not to A050694. */
/* The implementation searches integers in increasing order and tests
   whether they satisfy the digit‑containment condition for all prime
   factors but do not have all prime factors as substrings. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_counts.h"

/* Helper: test whether integer 'num' fulfills the sequence conditions. */
static bool qualifies(uint64_t num)
{
    /* prime factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);
    if (cnt == 0)         /* should not happen for num > 1 */
        return false;

    /* digits present in the number */
    unsigned char cntN[10] = {0};
    digit_counts((seq_t)num, cntN);
    bool hasDigit[10];
    for (int d = 0; d < 10; d++)
        hasDigit[d] = cntN[d] > 0;

    /* string representation of the number for substring checks */
    char numbuf[32];
    size_t nlen;
    seq_to_str((seq_t)num, numbuf, &nlen);
    numbuf[nlen] = '\0';

    bool all_substrings = true;

    for (size_t i = 0; i < cnt; i++)
    {
        uint64_t p = primes[i];

        /* digit condition for this prime factor */
        unsigned char cntP[10] = {0};
        digit_counts((seq_t)p, cntP);
        for (int d = 0; d < 10; d++)
            if (cntP[d] > 0 && !hasDigit[d])
                return false;      /* missing necessary digit */

        /* substring test for this prime factor */
        char pbuf[16];
        size_t plen;
        seq_to_str((seq_t)p, pbuf, &plen);
        pbuf[plen] = '\0';
        if (strstr(numbuf, pbuf) == NULL)
            all_substrings = false;
    }

    return !all_substrings;          /* must have at least one non‑sub‑factor */
}

/* dynamic array to store computed terms, permanently memoised */
static seq_t  *term_buffer = NULL;
static size_t term_cap   = 0;
static size_t term_len   = 0;
static uint64_t next_to_scan = 2;   /* next integer to test */

/* append a value to the term_buffer, growing it if necessary */
static void append_term(seq_t val)
{
    if (term_len == term_cap)
    {
        size_t new_cap = term_cap ? term_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(term_buffer, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        term_buffer = tmp;
        term_cap   = new_cap;
    }
    term_buffer[term_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined for n<1 */

    /* generate terms until we have at least n terms */
    while (term_len < (size_t)n)
    {
        if (qualifies(next_to_scan))
            append_term((seq_t)next_to_scan);
        next_to_scan++;
    }

    return term_buffer[n - 1];
}
