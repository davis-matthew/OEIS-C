/*
 * A228214  Number of second differences of arrays of length n + 2 of numbers
 *          in 0..4.
 * https://oeis.org/A228214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n+2) - 4^(n+2) for n > 7 (otherwise as listed)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Efficient binary exponentiation for seq_t types. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Sequence A228214. */
seq_t a(idx_t n)
{
    /* Undefined for indices below the offset; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Initial terms up to n = 7 inclusive. */
    static const seq_t init[7] = {
        (seq_t)17ULL,      /* a(1) */
        (seq_t)177ULL,     /* a(2) */
        (seq_t)1429ULL,    /* a(3) */
        (seq_t)9705ULL,    /* a(4) */
        (seq_t)58141ULL,   /* a(5) */
        (seq_t)320481ULL,  /* a(6) */
        (seq_t)1688101ULL  /* a(7) */
    };

    if (n <= 7)
        return init[n - 1];

    /* For n > 7 use the closed‑form expression. */
    idx_t exp = n + 2;
    seq_t pow5 = pow_seq((seq_t)5, exp);
    seq_t pow4 = pow_seq((seq_t)4, exp);
    return pow5 - pow4;
}
