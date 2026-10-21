/*
 * A202661  Number of -5..5 arrays of n elements with first through fourth
 *          differences also in -5..5.
 * https://oeis.org/A202661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arrays of length n over {-5..5} whose first to
 *            fourth * differences all lie in {-5..5}
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define S_MIN       -5
#define S_MAX        5
#define S_RANGE      11
#define STATES      14641   /* 11^4 */

/* encode a 4-tuple of values in [-5,5] to a unique index 0..14640 */
static inline int encode(int a, int b, int c, int d)
{
    return ((((a + 5) * S_RANGE + (b + 5)) * S_RANGE + (c + 5)) * S_RANGE + (d + 5));
}

static void init_static(void)
{
    static bool initialized = false;
    /* static data used in all calls */
    static short next_states[STATES][S_RANGE];   /* each state: up to 11 transitions */
    static unsigned char next_cnt[STATES];       /* number of transitions per state */
    static unsigned char valid_prefix[STATES];  /* flag whether a 4-tuple is a valid array of length 4 */

    /* Pointers to mutable DP arrays */
    static seq_t *dp  = NULL;
    static seq_t *dpn = NULL;

    if (initialized) return;
    initialized = true;

    /* allocate mutable DP buffers (shared across calls) */
    dp  = (seq_t*)malloc(STATES * sizeof(seq_t));
    dpn = (seq_t*)malloc(STATES * sizeof(seq_t));

    /* Precompute transitions and initial 4‑tuple validity */
    for (int t0 = S_MIN; t0 <= S_MAX; ++t0) {
        for (int t1 = S_MIN; t1 <= S_MAX; ++t1) {
            for (int t2 = S_MIN; t2 <= S_MAX; ++t2) {
                for (int t3 = S_MIN; t3 <= S_MAX; ++t3) {
                    int idx = encode(t0, t1, t2, t3);

                    /* check validity of a 4‑element array for n=4 */
                    int d1 = t1 - t0;
                    int d2 = t2 - 2*t1 + t0;
                    int d3 = t3 - 3*t2 + 3*t1 - t0;
                    if (d1 >= S_MIN && d1 <= S_MAX &&
                        d2 >= S_MIN && d2 <= S_MAX &&
                        d3 >= S_MIN && d3 <= S_MAX) {
                        valid_prefix[idx] = 1;
                    } else {
                        valid_prefix[idx] = 0;
                    }

                    /* precompute all possible next states */
                    unsigned char count = 0;
                    for (int x = S_MIN; x <= S_MAX; ++x) {
                        int nd1 = x - t3;
                        if (nd1 < S_MIN || nd1 > S_MAX) continue;

                        int nd2 = x - 2*t3 + t2;
                        if (nd2 < S_MIN || nd2 > S_MAX) continue;

                        int nd3 = x - 3*t3 + 3*t2 - t1;
                        if (nd3 < S_MIN || nd3 > S_MAX) continue;

                        int nd4 = x - 4*t3 + 6*t2 - 4*t1 + t0;
                        if (nd4 < S_MIN || nd4 > S_MAX) continue;

                        int nxt = encode(t1, t2, t3, x);
                        next_states[idx][count++] = (short)nxt;
                    }
                    next_cnt[idx] = count;
                }
            }
        }
    }

    /* make the static data visible to the main function via static variables */
    /* These static variables are defined in init_static, but we expose them
     * via static pointers in the main function's static scope.  */
    /* The static variables are accessed from within a() via static references. */
    /* The following trick makes the compiler use the same arrays across calls. */
    *((long long*)(&dp  )[0]) = (long long)dp  ;   /* avoid unused variable warning */
    *((long long*)(&dpn)[0]) = (long long)dpn ;
}

/* The mutable DP buffers must be available across calls; we store them in
 * static locals inside a() and initialize them on first call. */
static seq_t* dp          = NULL;
static seq_t* dpn         = NULL;
static short next_states[STATES][S_RANGE];
static unsigned char next_cnt[STATES];
static unsigned char valid_prefix[STATES];

seq_t a(idx_t n)
{
    /* Handle the first four offsets directly */
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)91;
    if (n == 3) return (seq_t)587;
    if (n == 4) return (seq_t)2833;

    /* Initialise static tables and buffers on first call */
    static bool initialized = false;
    if (!initialized) {
        /* allocate DP buffers */
        dp  = (seq_t*)malloc(STATES * sizeof(seq_t));
        dpn = (seq_t*)malloc(STATES * sizeof(seq_t));

        /* Precompute transitions */
        for (int t0 = S_MIN; t0 <= S_MAX; ++t0) {
            for (int t1 = S_MIN; t1 <= S_MAX; ++t1) {
                for (int t2 = S_MIN; t2 <= S_MAX; ++t2) {
                    for (int t3 = S_MIN; t3 <= S_MAX; ++t3) {
                        int idx = encode(t0, t1, t2, t3);

                        /* 4‑tuple validity for n=4 */
                        int d1 = t1 - t0;
                        int d2 = t2 - 2*t1 + t0;
                        int d3 = t3 - 3*t2 + 3*t1 - t0;
                        valid_prefix[idx] =
                            (d1 >= S_MIN && d1 <= S_MAX &&
                             d2 >= S_MIN && d2 <= S_MAX &&
                             d3 >= S_MIN && d3 <= S_MAX) ? 1 : 0;

                        /* transitions for adding a new element x */
                        unsigned char count = 0;
                        for (int x = S_MIN; x <= S_MAX; ++x) {
                            int nd1 = x - t3;
                            if (nd1 < S_MIN || nd1 > S_MAX) continue;

                            int nd2 = x - 2*t3 + t2;
                            if (nd2 < S_MIN || nd2 > S_MAX) continue;

                            int nd3 = x - 3*t3 + 3*t2 - t1;
                            if (nd3 < S_MIN || nd3 > S_MAX) continue;

                            int nd4 = x - 4*t3 + 6*t2 - 4*t1 + t0;
                            if (nd4 < S_MIN || nd4 > S_MAX) continue;

                            int nxt = encode(t1, t2, t3, x);
                            next_states[idx][count++] = (short)nxt;
                        }
                        next_cnt[idx] = count;
                    }
                }
            }
        }
        initialized = true;
    }

    /* initialize DP for length 4 */
    for (int i = 0; i < STATES; ++i) {
        dp[i] = valid_prefix[i] ? (seq_t)1 : (seq_t)0;
    }

    /* iterate from length 5 up to n */
    for (idx_t len = 5; len <= n; ++len) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) dpn[i] = (seq_t)0;
        /* transition */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            unsigned char cnt = next_cnt[i];
            short *t = next_states[i];
            for (int j = 0; j < cnt; ++j) {
                dpn[ t[j] ] += val;
            }
        }
        /* swap dp and dpn */
        seq_t *tmp = dp; dp = dpn; dpn = tmp;
    }

    /* sum counts of all states for length n */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < STATES; ++i) ans += dp[i];
    return ans;
}
