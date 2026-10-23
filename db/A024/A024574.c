/*
 * A024574  a(n) = Sum_{k=1..n} [ 1/{k*e} ] where {x} := x - [ x ].
 * https://oeis.org/A024574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = Sum_{k=1}^n floor(1 / frac(k*e)) where {x} is the fractional part of x */

seq_t a(idx_t n)
{
    /* High‑precision rational approximation of e‑2 : 0.71828182845904523536...  */
    /* A/Q ≈ e‑2 with Q = 10^17, error < 0.5·10⁻¹⁷.  */
    static const unsigned long long A = 71828182845904524ULL;   /* ≈ (e‑2)·10^17  */
    static const unsigned long long Q = 100000000000000000ULL;  /* 10^17          */

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* Compute the fractional part of k·(e‑2) as an integer modulo Q. */
        unsigned __int128 prod = (unsigned __int128)k * A;
        unsigned long long rem = (unsigned long long)(prod % Q);  /* 0 < rem < Q  */

        /* floor(1 / {k(e‑2)}) == floor(Q / rem)  */
        unsigned long long term = Q / rem;        /* integer division   */

        sum += term;               /* implicit conversion to seq_t  */
    }

    return sum;
}
