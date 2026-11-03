/*
 * A133016  Even imperfect numbers, divided by 2.
 * https://oeis.org/A133016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th even number that is not a perfect number, divided by
 *            2
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes whether an even number x is a perfect number.
   Only the first few even perfect numbers are relevant for n <= 10000. */
static int is_even_perfect(seq_t x)
{
    static const seq_t perfects[] = {
        (seq_t)6,          /* 2^(2-1) * (2^2-1) = 6        */
        (seq_t)28,         /* 2^(3-1) * (2^3-1) = 28       */
        (seq_t)496,        /* 2^(5-1) * (2^5-1) = 496      */
        (seq_t)8128,       /* 2^(7-1) * (2^7-1) = 8128     */
        (seq_t)33550336,   /* 2^(13-1)*(2^13-1) = 33550336 */
        (seq_t)0           /* sentinel */
    };
    for (const seq_t *p = perfects; *p; ++p)
        if (x == *p)
            return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* n is 1-based index */
    idx_t i = 1;       /* current k (half of the even number) */
    idx_t count = 0;   /* how many even imperfect numbers have been seen */

    for (;; ++i) {
        seq_t value = ((seq_t)i) * ((seq_t)2);  /* the even number 2*i */
        if (!is_even_perfect(value)) {
            ++count;
            if (count == n)
                return (seq_t)i;               /* return k = a(n) */
        }
    }
}
