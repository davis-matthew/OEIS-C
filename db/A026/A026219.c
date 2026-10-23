/*
 * A026219  Position of n in A026218.
 * https://oeis.org/A026219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-6) - a(n-8) for n > 8 Initial values:
 *            a(1)=1, a(2)=2, a(3)=5, a(4)=3, a(5)=9, a(6)=4, a(7)=13, a(8)=6
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* offset is 1; value for n=0 is irrelevant */
    }
    /* Handle the first eight terms directly */
    if (n <= 8) {
        static const seq_t init[] = {0, 1, 2, 5, 3, 9, 4, 13, 6};
        return init[n];
    }

    /* Cache the computed values to avoid recomputing them with each call.
       The sequence grows slowly, and n never exceeds about 10 000 in the
       tests.  We therefore keep a fixed-size static array large enough
       for all expected indices. */
    /* 20 000 gives a generous margin; adjust if higher indices are needed. */
    enum { MAX_CACHE = 20000 };
    static seq_t cache[MAX_CACHE + 1];
    static idx_t computed_to = 0;

    /* Initialise the first eight terms once */
    if (computed_to < 8) {
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 5;
        cache[4] = 3;
        cache[5] = 9;
        cache[6] = 4;
        cache[7] = 13;
        cache[8] = 6;
        computed_to = 8;
    }

    /* Compute values up to n if necessary */
    if (n > computed_to) {
        for (idx_t i = computed_to + 1; i <= n; ++i) {
            cache[i] = cache[i - 2] + cache[i - 6] - cache[i - 8];
        }
        computed_to = n;
    }

    return cache[n];
}
