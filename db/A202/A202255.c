/*
 * A202255  Number of zero-sum -n..n arrays of 5 elements with adjacent
 *          element differences also in -n..n.
 * https://oeis.org/A202255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) - a(n-6) - a(n-7) + a(n-8) +
 *            a(n-9) + a(n-10) - a(n-13) - a(n-14) + a(n-15), with a(1)=15,
 *            a(2)=107, a(3)=397, a(4)=1077, a(5)=2385, a(6)=4643, a(7)=8211,
 *            a(8)=13533, a(9)=21091, a(10)=31461, a(11)=45241, a(12)=63135,
 *            a(13)=85861, a(14)=114251, a(15)=149145
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 201, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cache holds terms up to a(20000),
       which covers all calls with n≤10000. */
    static seq_t cache[20001];
    static int initialized = 0;
    static idx_t last = 15;

    if (!initialized) {
        /* Initialize the first 15 terms */
        cache[0] = 0;            /* unused, but keeps array zero‑initialised */
        cache[1] = 15;
        cache[2] = 107;
        cache[3] = 397;
        cache[4] = 1077;
        cache[5] = 2385;
        cache[6] = 4643;
        cache[7] = 8211;
        cache[8] = 13533;
        cache[9] = 21091;
        cache[10] = 31461;
        cache[11] = 45241;
        cache[12] = 63135;
        cache[13] = 85861;
        cache[14] = 114251;
        cache[15] = 149145;
        initialized = 1;
    }

    /* Return early for the already initialised range. */
    if (n <= 15)
        return cache[n];

    /* Extend the cached range if needed. */
    if (last < 15)
        last = 15;

    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2]
                 - cache[i-5] - cache[i-6] - cache[i-7]
                 + cache[i-8] + cache[i-9] + cache[i-10]
                 - cache[i-13] - cache[i-14] + cache[i-15];
    }
    last = n;
    return cache[n];
}
