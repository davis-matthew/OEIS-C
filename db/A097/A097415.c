/*
 * A097415  Values of k such that the first digit of 2^k is 9.
 * https://oeis.org/A097415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2998, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A097415: a(n) is the nth integer k such that the first decimal
   digit of 2^k equals 9.  The computation uses the fractional part of
   k·log10(2) with a high‑precision integer approximation. */

#include <stdlib.h>

#define LOG10_2_NUM 301029995663981ULL   /* floor(log10(2)*1e15) */
#define LOG10_9_NUM 954242509439324ULL   /* floor(log10(9)*1e15) */
#define MOD          1000000000000000ULL /* 1e15 used as scaling factor */

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_cap = 0;
    static size_t seq_len = 0;
    static idx_t last_k = 0;        /* largest k examined so far */

    if (n == 0) return 0;           /* undefined for n = 0, but guard */

    if (n <= seq_len)               /* already computed */
        return seq[n - 1];

    /* Extend the sequence until we have at least n terms */
    while (seq_len < n) {
        ++last_k;                   /* test the next exponent k */

        /* Compute the fractional part of k·log10(2) using integers:
           f = (LOG10_2_NUM * k) mod MOD, represented as a number in [0,MOD). */
        unsigned __int128 prod = (unsigned __int128)LOG10_2_NUM * last_k;
        uint64_t f = (uint64_t)(prod % MOD);

        /* If the fractional part is at least log10(9), k has leading digit 9. */
        if (f >= LOG10_9_NUM) {
            /* Ensure capacity before inserting a new term. */
            if (seq_cap <= seq_len) {
                size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
                while (new_cap <= seq_len) new_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);           /* allocation failure */
                seq = tmp;
                seq_cap = new_cap;
            }
            seq[seq_len++] = (seq_t)last_k;
        }
    }

    return seq[n - 1];
}
