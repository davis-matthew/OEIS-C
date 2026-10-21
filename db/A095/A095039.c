/*
 * A095039  An example of a (v,k,lambda)=(40,13,4) cyclic difference set.
 * https://oeis.org/A095039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = 40 * ((n-1)/13) + D[(n-1) mod 13] where D = {0, 1, 3, 5,
 *            9, 15, 22, 25, 26, 27, 34, 35, 38}
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t D[13] = {
    0ULL, 1ULL, 3ULL, 5ULL, 9ULL, 15ULL,
    22ULL, 25ULL, 26ULL, 27ULL, 34ULL, 35ULL, 38ULL
};

seq_t a(idx_t n)
{
    idx_t base = (n - 1) / 13;
    idx_t pos  = (n - 1) % 13;
    return (seq_t)base * (seq_t)40 + D[pos];
}
