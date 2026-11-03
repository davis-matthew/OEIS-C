/*
 * A031445  Numbers whose base-2 representation has 2 more 0's than 1's.
 * https://oeis.org/A031445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer whose binary representation has exactly
 *            two more 0’s than 1’s
 * Range:     u8 n <= 27, u16 n <= 6733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9791 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* binom_cap(n,k,cap) returns C(n,k) but never exceeds cap+1.
   It uses seq_t for intermediate calculations, but caps the result
   to avoid overflow when only a comparison to cap is needed. */
static idx_t binom_cap(idx_t n, idx_t k, idx_t cap)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / (seq_t)i;
        if (res > cap) return cap + 1;   /* exceeded cap */
    }
    return (idx_t)res;
}

/* unrank(L, idx) produces the idx‑th number of length L
   (0‑based count).  L is even and at least 4. */
static seq_t unrank(idx_t L, idx_t rank)
{
    /* Number of 1’s needed besides the leading one.
       Total ones for length L: L/2 – 1. */
    idx_t needOnes = (L / 2) - 2;
    seq_t value = ((seq_t)1) << (L - 1);   /* leading 1 */

    if (needOnes == 0) return value;

    for (idx_t pos = L - 2; pos + 1; --pos) {
        if (needOnes == 0) break;
        idx_t cntZero = binom_cap(pos, needOnes, rank);
        if (rank < cntZero) {
            /* keep bit 0 */
        } else {
            /* set bit 1 */
            value |= ((seq_t)1 << pos);
            rank -= cntZero;
            --needOnes;
        }
    }
    return value;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; n==0 is undefined but harmless */

    idx_t remaining = n;    /* how many items still to skip */
    idx_t L = 4;            /* smallest even length that can occur */

    for (;;) {
        if (L < 4) continue;                  /* safety */
        /* Number of 1's in total for this length:
           ones = L/2 – 1.  Leading 1 uses one of them. */
        idx_t onesTotal = (L / 2) - 1;
        if (onesTotal < 1) { L += 2; continue; }

        /* Count of numbers with this length:
           choose positions of the remaining (onesTotal‑1) ones among L‑1 bits. */
        idx_t count = binom_cap(L - 1, onesTotal - 1, remaining);

        if (remaining <= count) {
            return unrank(L, remaining - 1);  /* 0‑based rank */
        }
        remaining -= count;
        L += 2;   /* next even length */
    }
}
