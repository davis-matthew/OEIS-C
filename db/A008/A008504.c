/*
 * A008504  10-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+11,11) - binom(n,11) Equivalently, a(0)=1; for
 *            n≥1, a(n) = a(n-1) + binom(n+10,10) - binom(n-1,10)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 31, u64 n <= 300, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_div(seq_t a, seq_t num, seq_t den)
{
#if defined(__SIZEOF_INT128__)
    __int128 tmp = (__int128)a * (__int128)num;
    return (seq_t)(tmp / den);
#else
    /* Fallback for environments without 128‑bit support.
       The intermediate result is kept below 64 bits because
       'a' and 'num' will never exceed the 64‑bit limit in this
       algorithm. */
    return (seq_t)((unsigned long long)a * num / den);
#endif
}

seq_t a(idx_t n)
{
    seq_t res = 1;                      /* a(0) */
    if (n == 0) return res;

    seq_t binom_up   = 1;                 /* C(10,10)  */
    seq_t binom_down = 0;                 /* C(0,10)   */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* C(i+10,10) */
        seq_t new_up = mul_div(binom_up, (seq_t)i + 10, (seq_t)i);

        /* C(i-1,10) */
        seq_t new_down = binom_down;
        if (i == 11)            new_down = 1;
        else if (i > 11)         /* C(i-1,10) = C(i-2,10)*(i-1)/(i-11) */
            new_down = mul_div(binom_down, (seq_t)(i - 1), (seq_t)(i - 11));

        res += new_up - new_down;

        binom_up   = new_up;
        binom_down = new_down;
    }

    return res;
}
