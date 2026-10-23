/*
 * A022795  Place where n-th 1 occurs in A023133.
 * https://oeis.org/A022795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n-1} ceil(pi*k) (pi = 3.141592653589793…)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 14‑digit truncated value of pi: 3.14159265358979. */
static const seq_t PI_NUM = 314159265358979ULL; /* numerator  pi*10^14  */
static const seq_t PI_DEN = 100000000000000ULL; /* denominator 10^14       */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                    /* a(1) = 1 */

    seq_t sum = 1;                    /* a(1) */
    seq_t floor_val, ceil_val;

    for (idx_t k = 1; k < n; ++k) {   /* k = 1 … n-1  */
        /* floor(k * pi)  ≈ floor(k * PI_NUM / PI_DEN) */
        floor_val = ((seq_t)k * PI_NUM) / PI_DEN;
        ceil_val  = floor_val + 1;    /* ceil(k * pi) = floor(k * pi) + 1  */
        sum += ceil_val;
    }

    return sum;
}
