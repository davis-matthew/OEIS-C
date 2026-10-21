/*
 * A237378  Number of (n+1) X (2+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 1320 * a(n-4) for n > 6;
   Initial values: a(1) = 344, a(2) = 2064, a(3) = 12408,
   a(4) = 76188, a(5) = 455160, a(6) = 2721900. */

seq_t a(idx_t n)
{
    /* Base cases for 1 ≤ n ≤ 6. */
    static const seq_t init[6] = {
        (seq_t)344,
        (seq_t)2064,
        (seq_t)12408,
        (seq_t)76188,
        (seq_t)455160,
        (seq_t)2721900
    };

    if (n <= 6)
        return init[n - 1];

    /* Determine the offset within the 4‑step cycle. */
    idx_t r;
    /* n is unsigned, so modulus is well defined. */
    r = n % (idx_t)4;

    idx_t base;   /* base index (1‑based) from which to start the recurrence */
    switch (r) {
        case 0:  base = 4; break;  /* n ≡ 0 (mod 4) → a(4) */
        case 1:  base = 5; break;  /* n ≡ 1 (mod 4) → a(5) */
        case 2:  base = 6; break;  /* n ≡ 2 (mod 4) → a(6) */
        case 3:  base = 3; break;  /* n ≡ 3 (mod 4) → a(3) */
        default: base = 4;         /* unreachable, keeps compiler quiet */
    }

    /* Number of recurrence steps needed to reach n from the base index. */
    idx_t k = (n - base) / 4;

    /* Compute 1320^k using binary exponentiation to keep the number of
       multiplications logarithmic in k. The intermediate value `power`
       may grow beyond 64 bits if seq_t is an arbitrary‑precision type,
       which the compiler will handle correctly. */
    seq_t multiplier = 1;
    seq_t power = (seq_t)1320;
    idx_t exp = k;
    while (exp) {
        if (exp & 1)
            multiplier *= power;
        power *= power;
        exp >>= 1;
    }

    return init[base - 1] * multiplier;
}
