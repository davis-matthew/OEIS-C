/*
 * A059738  Binomial transform of A054341 and inverse binomial transform of
 *          A049027.
 * https://oeis.org/A059738
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
   a(0) = 1, a(1) = 3, a(2) = 10;
   for n ≥ 3
       2·(n+1)·a(n) = (11·n+5)·a(n‑1) – (8·n+5)·a(n‑2) – 21·(n‑2)·a(n‑3)
   */
seq_t a(idx_t n)
{
    /* Handle the three initial values immediately. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)10;

    /* a(n‑3), a(n‑2), a(n‑1) */
    seq_t a_m3 = (seq_t)1;
    seq_t a_m2 = (seq_t)3;
    seq_t a_m1 = (seq_t)10;

    for (idx_t i = 3; i <= n; ++i) {
#ifdef __cplusplus
        /* C++ build: seq_t is an arbitrary‑precision class, use
           ordinary operators. */
        seq_t term1 = a_m1 * (seq_t)(11*i + 5);
        seq_t term2 = a_m2 * (seq_t)(8*i  + 5);
        seq_t term3 = a_m3 * (seq_t)(21*(i-2));
        seq_t num   = term1 - term2 - term3;
        seq_t denom = (seq_t)(2*(i+1));
        seq_t next  = num / denom;
#else
        /* C build: seq_t is a builtin unsigned integer.
           Use 128‑bit arithmetic to avoid overflow on intermediate
           products while keeping the final result in seq_t. */
        unsigned __int128 coeff1 = (unsigned __int128)(11*i + 5);
        unsigned __int128 coeff2 = (unsigned __int128)(8*i  + 5);
        unsigned __int128 coeff3 = (unsigned __int128)(21*(i-2));

        unsigned __int128 prod1 = (unsigned __int128)a_m1 * coeff1;
        unsigned __int128 prod2 = (unsigned __int128)a_m2 * coeff2;
        unsigned __int128 prod3 = (unsigned __int128)a_m3 * coeff3;

        unsigned __int128 num   = prod1 - prod2 - prod3;
        unsigned __int128 denom = (unsigned __int128)2 * (i+1);
        seq_t next = (seq_t)(num / denom);
#endif
        /* Shift the sliding window */
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = next;
    }
    return a_m1;
}
