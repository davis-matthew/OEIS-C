/*
 * A108178  a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)(7n^2 + 23n + 20)/2880.
 * https://oeis.org/A108178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^2*(n+3)^2*(n+4)(7n^2 + 23n + 20)/2880
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 31, u64 n <= 540, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper to cancel factors of 2, 3 and 5 from a value */
static inline void reduce_factor(seq_t *v, int *twos, int *threes, int *fives)
{
    while (*twos > 0 && (*v & 1) == 0) {
        *v >>= 1;
        (*twos)--;
    }
    while (*threes > 0 && *v % 3 == 0) {
        *v /= 3;
        (*threes)--;
    }
    while (*fives > 0 && *v % 5 == 0) {
        *v /= 5;
        (*fives)--;
    }
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    int twos   = 6;  /* 2^6 = 64 */
    int threes = 2;  /* 3^2 = 9 */
    int fives  = 1;  /* 5   */
    seq_t result = 1;
    seq_t v;

    /* (n+1) */
    v = N + 1;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;

    /* (n+2)^2  -> two copies of (n+2) */
    v = N + 2;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;
    v = N + 2;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;

    /* (n+3)^2  -> two copies of (n+3) */
    v = N + 3;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;
    v = N + 3;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;

    /* (n+4) */
    v = N + 4;
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;

    /* 7n^2 + 23n + 20 */
    v = (seq_t)(7 * N * N + 23 * N + 20);
    reduce_factor(&v, &twos, &threes, &fives);
    result *= v;

    /* any remaining denominator factors */
    while (twos-- > 0) result /= 2;
    while (threes-- > 0) result /= 3;
    while (fives-- > 0) result /= 5;

    return result;
}
