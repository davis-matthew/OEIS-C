/*
 * A229387  Number of n X 3 0..3 arrays of the sum of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 4 0..1 array.
 * https://oeis.org/A229387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 25*a(n-1) -152*a(n-2) +144*a(n-3) * -368*a(n-4)
 *            +1888*a(n-5) -1536*a(n-6) for n>9 * Initial values: * a(1)=48,
 *            a(2)=1064, a(3)=19124, a(4)=319340, * a(5)=5212236,
 *            a(6)=84210828, a(7)=1353901580, * a(8)=21715025932,
 *            a(9)=347864379404
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE 10000            /* sufficient for tests up to n≈10000 */

static seq_t cache[MAX_CACHE];     /* memoised values, 1‑based in terms of n */
static idx_t cached_n = 0;         /* number of entries currently computed */

/* first 9 exact values */
static const seq_t init[9] = {
    48ULL, 1064ULL, 19124ULL, 319340ULL, 5212236ULL,
    84210828ULL, 1353901580ULL, 21715025932ULL, 347864379404ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset guard; not part of sequence */

    /* Handle trivial cases directly */
    if (n <= 9) return init[n - 1];

    /* Initialize cache on first use */
    if (cached_n == 0) {
        for (idx_t i = 0; i < 9; ++i)
            cache[i] = init[i];
        cached_n = 9;
    }

    /* If the value is already known, return it */
    if (cached_n >= n)
        return cache[n - 1];

    /* For n within the pre‑allocated cache size, compute and store */
    if (n <= MAX_CACHE) {
        for (idx_t i = cached_n; i < n; ++i) {
            seq_t val =
                25ULL * cache[i - 1]
                - 152ULL * cache[i - 2]
                + 144ULL * cache[i - 3]
                - 368ULL * cache[i - 4]
                + 1888ULL * cache[i - 5]
                - 1536ULL * cache[i - 6];
            cache[i] = val;
        }
        cached_n = n;
        return cache[n - 1];
    }

    /* For n larger than MAX_CACHE, compute iteratively without saving everything. */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i)
        buf[i] = init[i];

    for (idx_t m = 9; m < n; ++m) {
        seq_t val =
            25ULL * buf[8]
            - 152ULL * buf[7]
            + 144ULL * buf[6]
            - 368ULL * buf[5]
            + 1888ULL * buf[4]
            - 1536ULL * buf[3];

        /* shift left: drop the oldest term, make room for the new one */
        for (int k = 0; k < 8; ++k) buf[k] = buf[k + 1];
        buf[8] = val;
    }
    return buf[8];
}
