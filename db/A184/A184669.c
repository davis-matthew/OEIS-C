/*
 * A184669  Number of (n+1)X6 0..3 arrays with no 2X2 subblock summing to less
 *          than 6.
 * https://oeis.org/A184669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays with entries 0..3 that contain
 *            no 2×2 subblock whose four cells sum to less than 6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation models each row by the 5 adjacent‑column pair sums
   (values 0…6).  Two rows are compatible iff every pair of corresponding
   sums adds to at least 6.  The state space is 7^5 = 16807.  The transition
   matrix is the Kronecker product of a 7×7 matrix A where A[i][j] = 1
   iff i+j≥6.  A DP term for n rows is obtained by repeatedly applying
   the 5 independent one‑dimensional transforms, then multiplying by the
   multiplicity of each row pattern.  Memoisation allows arbitrary
   order of queries.  */

#include <stdlib.h>

static const int pow7[6] = {1, 7, 49, 343, 2401, 16807};

/* One‑dimensional transform:  src → dst on dimension 'dim' (0…4).
   stride = 7^(4-dim).  The transform implements the local constraint
   that the sum of the two adjacent pairs from the previous row and the
   current row is at least 6.  */
static void transform_dim(const seq_t *src, seq_t *dst, int dim)
{
    int stride = pow7[4 - dim];
    switch (dim) {
    case 0: { /* dimension 0 */
        for (int i1 = 0; i1 < 7; ++i1)
            for (int i2 = 0; i2 < 7; ++i2)
                for (int i3 = 0; i3 < 7; ++i3)
                    for (int i4 = 0; i4 < 7; ++i4) {
                        int base = i1 * pow7[3] + i2 * pow7[2] + i3 * pow7[1] + i4;
                        seq_t pref[8];
                        pref[0] = (seq_t)0;
                        for (int j = 0; j < 7; ++j)
                            pref[j + 1] = pref[j] + src[base + j * stride];
                        for (int t = 0; t < 7; ++t) {
                            int thr = 6 - t;
                            if (thr < 0) thr = 0;
                            dst[base + t * stride] = pref[7] - pref[thr];
                        }
                    }
        break;
    }
    case 1: { /* dimension 1 */
        for (int i0 = 0; i0 < 7; ++i0)
            for (int i2 = 0; i2 < 7; ++i2)
                for (int i3 = 0; i3 < 7; ++i3)
                    for (int i4 = 0; i4 < 7; ++i4) {
                        int base = i0 * pow7[4] + i2 * pow7[2] + i3 * pow7[1] + i4;
                        seq_t pref[8];
                        pref[0] = (seq_t)0;
                        for (int j = 0; j < 7; ++j)
                            pref[j + 1] = pref[j] + src[base + j * stride];
                        for (int t = 0; t < 7; ++t) {
                            int thr = 6 - t;
                            if (thr < 0) thr = 0;
                            dst[base + t * stride] = pref[7] - pref[thr];
                        }
                    }
        break;
    }
    case 2: { /* dimension 2 */
        for (int i0 = 0; i0 < 7; ++i0)
            for (int i1 = 0; i1 < 7; ++i1)
                for (int i3 = 0; i3 < 7; ++i3)
                    for (int i4 = 0; i4 < 7; ++i4) {
                        int base = i0 * pow7[4] + i1 * pow7[3] + i3 * pow7[1] + i4;
                        seq_t pref[8];
                        pref[0] = (seq_t)0;
                        for (int j = 0; j < 7; ++j)
                            pref[j + 1] = pref[j] + src[base + j * stride];
                        for (int t = 0; t < 7; ++t) {
                            int thr = 6 - t;
                            if (thr < 0) thr = 0;
                            dst[base + t * stride] = pref[7] - pref[thr];
                        }
                    }
        break;
    }
    case 3: { /* dimension 3 */
        for (int i0 = 0; i0 < 7; ++i0)
            for (int i1 = 0; i1 < 7; ++i1)
                for (int i2 = 0; i2 < 7; ++i2)
                    for (int i4 = 0; i4 < 7; ++i4) {
                        int base = i0 * pow7[4] + i1 * pow7[3] + i2 * pow7[2] + i4;
                        seq_t pref[8];
                        pref[0] = (seq_t)0;
                        for (int j = 0; j < 7; ++j)
                            pref[j + 1] = pref[j] + src[base + j * stride];
                        for (int t = 0; t < 7; ++t) {
                            int thr = 6 - t;
                            if (thr < 0) thr = 0;
                            dst[base + t * stride] = pref[7] - pref[thr];
                        }
                    }
        break;
    }
    case 4: { /* dimension 4 */
        for (int i0 = 0; i0 < 7; ++i0)
            for (int i1 = 0; i1 < 7; ++i1)
                for (int i2 = 0; i2 < 7; ++i2)
                    for (int i3 = 0; i3 < 7; ++i3) {
                        int base = i0 * pow7[4] + i1 * pow7[3] + i2 * pow7[2] + i3 * pow7[1];
                        seq_t pref[8];
                        pref[0] = (seq_t)0;
                        for (int j = 0; j < 7; ++j)
                            pref[j + 1] = pref[j] + src[base + j * stride];
                        for (int t = 0; t < 7; ++t) {
                            int thr = 6 - t;
                            if (thr < 0) thr = 0;
                            dst[base + t * stride] = pref[7] - pref[thr];
                        }
                    }
        break;
    }
    }
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    /* 1. static data for all calls */
    static int      initialized = 0;
    static seq_t   *cnt  = NULL;
    static seq_t   *dp   = NULL;
    static seq_t   *t1   = NULL;
    static seq_t   *t2   = NULL;
    static seq_t   *t3   = NULL;
    static seq_t   *t4   = NULL;
    static seq_t   *t5   = NULL;
    static seq_t   *memo = NULL;
    static int      memo_cap = 20001;     /* enough for n up to ~2e4 */
    static int      memo_len = 0;         /* highest n computed */
    static const int N = 16807;           /* 7^5 */

    /* 2. one‑time initialisation */
    if (!initialized) {
        cnt  = (seq_t*)calloc(N, sizeof(seq_t));
        dp   = (seq_t*)malloc(N * sizeof(seq_t));
        t1   = (seq_t*)malloc(N * sizeof(seq_t));
        t2   = (seq_t*)malloc(N * sizeof(seq_t));
        t3   = (seq_t*)malloc(N * sizeof(seq_t));
        t4   = (seq_t*)malloc(N * sizeof(seq_t));
        t5   = (seq_t*)malloc(N * sizeof(seq_t));
        memo = (seq_t*)calloc(memo_cap, sizeof(seq_t));

        /* Count how many 6‑digit rows map to each 5‑tuple of adjacent sums. */
        for (int r = 0; r < 4096; ++r) {
            int digit[6];
            int x = r;
            for (int i = 0; i < 6; ++i) {
                digit[i] = x & 3;
                x >>= 2;
            }
            int idx = 0;
            for (int i = 0; i < 5; ++i) {
                int s = digit[i] + digit[i + 1];
                idx = idx * 7 + s;
            }
            cnt[idx] += (seq_t)1;
        }

        /* dp holds the number of arrays that end in each row pattern after each step. */
        for (int i = 0; i < N; ++i)
            dp[i] = cnt[i];

        initialized = 1;
        memo_len = 0;
    }

    /* 3. compute up to required n */
    if ((int)n > memo_len) {
        for (int k = memo_len + 1; k <= (int)n; ++k) {
            /* successive one‑dimensional transforms = full transition */
            transform_dim(dp, t1, 0);
            transform_dim(t1, t2, 1);
            transform_dim(t2, t3, 2);
            transform_dim(t3, t4, 3);
            transform_dim(t4, t5, 4);

            /* multiply by multiplicities to obtain arrays of length k+1 */
            for (int i = 0; i < N; ++i)
                dp[i] = t5[i] * cnt[i];

            /* sum of all states is the required a(k) */
            seq_t sum = (seq_t)0;
            for (int i = 0; i < N; ++i)
                sum += dp[i];

            if (k < memo_cap)
                memo[k] = sum;
        }
        memo_len = (int)n;
    }

    return memo[n];
}
