/*
 * A152654  A vector recursion sequence: k = 1; m = 2; l = 1;
 *          a(n)=k*{0,a(n-2),0}+m*{-(m-1)/m,a(n-1)}++m*{a(n-1),-(m-1)/m}+l*{0,0,a(n-4),0,0}.
 * https://oeis.org/A152654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) is obtained by concatenating the vectors V(k) for k =
 *            0,1,2,… where V(0) = {1} V(1) = {1,1} V(k)[0] = 2*V(k-1)[0] - 1
 *            V(k)[k] = 2*V(k-1)[k-1] - 1 V(k)[i] = V(k-2)[i-1] +
 *            2*V(k-1)[i-1] + 2*V(k-1)[i] + (k≥4 && i≥2 && i≤k-2 ? V(k-4)[i-2]
 *            : 0) for 1 ≤ i ≤ k-1. a(n) is the n‑th element of the seq...
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_K 200

static seq_t V[MAX_K + 1][MAX_K + 1];
static idx_t prefix[MAX_K + 1];
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* Base vectors */
        V[0][0] = 1;
        V[1][0] = 1;
        V[1][1] = 1;

        /* Pre‑compute all V(k) up to MAX_K */
        for (idx_t k = 2; k <= MAX_K; ++k)
        {
            /* first and last elements */
            V[k][0] = V[k - 1][0] * 2 - 1;
            V[k][k] = V[k - 1][k - 1] * 2 - 1;

            for (idx_t i = 1; i < k; ++i)
            {
                seq_t sum = V[k - 2][i - 1];
                sum += V[k - 1][i - 1] * 2;
                sum += V[k - 1][i] * 2;
                if (k >= 4 && i >= 2 && i <= k - 2)
                    sum += V[k - 4][i - 2];
                V[k][i] = sum;
            }
        }

        /* Prefix sums of lengths |V(k)| = k+1 */
        prefix[0] = 1;
        for (idx_t k = 1; k <= MAX_K; ++k)
            prefix[k] = prefix[k - 1] + (k + 1);

        initialized = 1;
    }

    /* Find the block that contains n */
    for (idx_t k = 0; k <= MAX_K; ++k)
    {
        if (n < prefix[k])
        {
            idx_t offset = (k == 0) ? n : n - prefix[k - 1];
            return V[k][offset];
        }
    }

    /* n exceeds precomputed range (unlikely for given constraints) */
    return 1;
}
