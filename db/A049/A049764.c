/*
 * A049764  a(n) = Sum_{k=1..n} T(n,k), array T as in A049763.
 * https://oeis.org/A049764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (n^4 mod i)
 * Range:     u8 n <= 37, u16 n <= 523, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t r = n % i;               /* r < i */
        unsigned __int128 t = r;       /* use 128‑bit for safe mul */
        t = t * r % i;                 /* r^2 mod i */
        t = t * r % i;                 /* r^3 mod i */
        t = t * r % i;                 /* r^4 mod i */
        sum += (seq_t)(unsigned long long)t; /* add to running sum */
    }
    return sum;
}
