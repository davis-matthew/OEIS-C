/*
 * A194423  Numbers m such that Sum_{k=1..m} (<2/3 + k*r> - <k*r>) = 0, where
 *          r=sqrt(2) and < > denotes fractional part.
 * https://oeis.org/A194423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal m such that Σ_{k=1..m} (⟨k*r+2/3⟩–⟨k*r⟩)=0, r =
 *            √2. Equivalently, let f_k = 1 if ⟨k√2⟩<1/3 else 0. Then
 *            3·Σ_{k=1..m} f_k – m = 0, so we locate indices m where the walk
 *            H(m)=3·Σ f_k – m revisits zero. Since f_k is just the indicator
 *            of **{k√2}<1/3**, we can compute it using only int...
 * Range:     u8 n <= 55, u16 n <= 2043, u32 n <= 2043, u64 n <= 2043,
 *            u128 n <= 2043, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* integer square root of a __int128 in [0, 2^64) */
static unsigned long long isqrt128(__int128 x)
{
    unsigned long long lo = 0, hi = 1ULL << 32;          /* sqrt < 2^32 for our k range */
    while (lo < hi) {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* compute a(n) for OEIS sequence A194423 */
seq_t a(idx_t n)
{
    /* static state across calls */
    static seq_t *seq = NULL;        /* array of found terms   */
    static idx_t count = 0;          /* number of terms stored */
    static idx_t capacity = 0;       /* current array capacity*/
    static idx_t last_checked = 0;   /* highest k processed   */
    static long long H = 0;          /* cumulative value 3*sum - m   */

    while (count < n) {
        ++last_checked;
        idx_t k = last_checked;
        __int128 kk = (__int128)k;

        /* compute floor(sqrt(2*k^2)) */
        __int128 t = kk * kk * 2;
        unsigned long long n_sqrt = isqrt128(t);

        /* f_k = 1 iff 18*k^2 < 9*n_sqrt^2 + 6*n_sqrt + 1 */
        __int128 left  = (__int128)18 * kk * kk;
        __int128 right = (__int128)9 * (__int128)n_sqrt * n_sqrt +
                         (__int128)6 * (__int128)n_sqrt + 1;
        bool fk = left < right;

        H += fk ? 2 : -1;
        if (H == 0) {
            /* store new term m = k */
            if (count == capacity) {
                capacity = capacity ? capacity * 2 : 64;
                seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
            }
            seq[count++] = (seq_t)k;
        }
    }
    return seq[n - 1];
}
