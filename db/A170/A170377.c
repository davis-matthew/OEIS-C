/*
 * A170377  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a(0)=1, a(1)=32, a(2)=992, …, and for n ≥ 0 a(n)
 *            = 30·∑_{k=1}^{min(42,n)} a(n−k) – (n≥43 ? 465·a(n−43) : 0) +
 *            p(n) where p(0)=1, p(1…42)=2, p(43)=1, p(n>43)=0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is valid C11 and C++17. */

#define MAX_CACHE 10001   /* sufficient for n up to ~10 000 */

static seq_t cache[MAX_CACHE];
static idx_t cached_up_to = 0;   /* next index to compute */

/* Return a(n) for the Coxeter group on 32 generators with
   (S_i)^2 = 1 and (S_iS_j)^43 = 1.  */
seq_t a(idx_t n)
{
    if (n + 1 > cached_up_to) {
        /* Extend the cache up to n.  */
        idx_t start = cached_up_to;
        if (start == 0) start = 0;          /* ensures we compute from 0 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = 0;
            idx_t kmax = (i < 42) ? i : 42;
            for (idx_t k = 1; k <= kmax; ++k) {
                sum += cache[i - k] * (seq_t)30;
            }
            if (i >= 43) {
                sum -= cache[i - 43] * (seq_t)465;
            }
            /* p(i) contribution */
            seq_t pi = 0;
            if (i == 0) {
                pi = 1;
            } else if (i >= 1 && i <= 42) {
                pi = 2;
            } else if (i == 43) {
                pi = 1;
            }
            cache[i] = sum + pi;
        }
        cached_up_to = n + 1;
        if (cached_up_to > MAX_CACHE) cached_up_to = MAX_CACHE; /* safety */
    }
    return cache[n];
}
