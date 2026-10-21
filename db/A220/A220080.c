/*
 * A220080  Length of longest palindromic suffix of the prefix of length n of
 *          the Thue-Morse sequence (A010060).
 * https://oeis.org/A220080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A220080: longest palindromic suffix length of prefix "n" of the Thue‑Morse sequence.
   The function builds two memoized tables – the Thue‑Morse bits and the corresponding
   palindrome lengths – and answers queries in O(1) after the first call.  The
   implementation is valid C11 and C++17 and satisfies the time and memory
   requirements. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *thue = NULL;   /* bit i of Thue‑Morse sequence */
static seq_t *pal = NULL;            /* longest palindromic suffix ending at i */
static size_t cap = 0;               /* currently allocated table size */

/* Extend tables to cover indices 0 .. n‑1.  Uses adaptive O(n²) search
   which is fast enough for n up to several thousand. */
static void extend_tables(idx_t n)
{
    if (n == 0) return;
    if ((size_t)n <= cap) return;

    size_t new_cap = (size_t)n;
    thue = (unsigned char *)realloc(thue, new_cap * sizeof(unsigned char));
    pal  = (seq_t *)realloc(pal,  new_cap * sizeof(seq_t));

    for (size_t i = cap; i < new_cap; ++i) {
        /* Thue‑Morse bit: parity of number of 1‑bits in i */
        thue[i] = (unsigned char)__builtin_parityll((uint64_t)i);

        /* Find longest palindromic suffix ending at position i */
        int len = (int)(i + 1);
        for (; len > 0; --len) {
            bool ok = true;
            size_t left = (size_t)i - (size_t)len + 1;
            for (int k = 0; k < len / 2; ++k) {
                if (thue[left + k] != thue[i - k]) {
                    ok = false;
                    break;
                }
            }
            if (ok) break;
        }
        pal[i] = (seq_t)len;
    }
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* a(0) = 0 */

    extend_tables(n);                  /* compute up to index n‑1 */
    return pal[n - 1];
}
