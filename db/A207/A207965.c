/*
 * A207965  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 7×n binary arrays avoiding * horizontally 0 0 0
 *            and 1 0 1, * vertically 0 0 1 and 1 0 0. * This implementation
 *            uses an automaton on pairs of columns * and computes the
 *            sequence up to any requested n. * The array is generated in
 *            linear time with respect * to n and uses about 2 MB...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* maximum possible state counts (2^7) */
#define MAX_STATE 128
/* safety limit for computed terms (sequence terms up to about 10 000) */
#define MAX_SEQ_LEN 20000

/* static data used by the sequence routine */
static int S;                         /* number of valid column states */
static uint8_t colState[MAX_STATE];   /* bit patterns for column states */

/* transition data: for each pair (i,j) we store how many k are allowed
 * and the list of such k's.  Index of pair is i*S + j.  */
static uint8_t  nextCnt[MAX_STATE*MAX_STATE];
static uint8_t  nextIdx[MAX_STATE*MAX_STATE*MAX_STATE];

/* dynamic programming arrays for the automaton */
static seq_t dpCur[MAX_STATE*MAX_STATE];
static seq_t dpNext[MAX_STATE*MAX_STATE];

/* sequence cache */
static seq_t seq_arr[MAX_SEQ_LEN];
static size_t seq_len;

/* initialization flag */
static int initialized = 0;

/* forward declaration of init function */
static void seq_init(void);

/* main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        seq_init();

    /* trivial requests */
    if (n == 0)
        return 0;

    if (n <= seq_len)
        return seq_arr[n-1];

    /* extend the cache up to n */
    while (seq_len < n) {
        /* compute next dp array */
        size_t i;
        for (i = 0; i < (size_t)S*S; ++i)
            dpNext[i] = 0;

        for (i = 0; i < (size_t)S*S; ++i) {
            seq_t ways = dpCur[i];
            if (!ways) continue;
            uint8_t cnt = nextCnt[i];
            uint8_t *list = &nextIdx[i*S];
            for (uint8_t t = 0; t < cnt; ++t) {
                uint8_t k = list[t];
                /* new pair is (prev1, k) where prev1 = i % S */
                size_t idx = ((size_t)(i % S) * S) + k;
                dpNext[idx] += ways;
            }
        }

        /* swap the dp arrays */
        for (i = 0; i < (size_t)S*S; ++i)
            dpCur[i] = dpNext[i];

        /* sum over all states to get a(new_len) */
        seq_t sum = 0;
        for (i = 0; i < (size_t)S*S; ++i)
            sum += dpCur[i];

        if (seq_len == MAX_SEQ_LEN) {
            /* shouldn't happen for problem limits */
            seq_arr[seq_len-1] = sum; /* keep last value */
        } else {
            seq_arr[seq_len] = sum;
        }
        ++seq_len;
    }

    return seq_arr[n-1];
}

/* initialisation routine that builds the automaton and
 * precomputes the first two terms. */
static void seq_init(void)
{
    /* --- compute valid column states (vertical constraint) --- */
    int cnt = 0;
    for (int v = 0; v < 128; ++v) {
        int ok = 1;
        for (int i = 0; i <= 4; ++i) {
            int b0 = (v >> i) & 1;
            int b1 = (v >> (i+1)) & 1;
            int b2 = (v >> (i+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 0 && b2 == 0)) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            colState[cnt++] = (uint8_t)v;
        }
    }
    S = cnt;
    /* --- precompute all transitions between pairs of states --- */
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            int pi = i*S + j;
            nextCnt[pi] = 0;
            for (int k = 0; k < S; ++k) {
                int ok = 1;
                for (int r = 0; r < 7; ++r) {
                    int bi = (colState[i] >> r) & 1;
                    int bj = (colState[j] >> r) & 1;
                    int bk = (colState[k] >> r) & 1;
                    if ((bi == 0 && bj == 0 && bk == 0) ||
                        (bi == 1 && bj == 0 && bk == 1)) {
                        ok = 0;
                        break;
                    }
                }
                if (ok) {
                    nextIdx[pi*S + nextCnt[pi]] = (uint8_t)k;
                    nextCnt[pi]++;
                }
            }
        }
    }
    /* --- initialise dp arrays for length 2 (any pair of columns) --- */
    for (int i = 0; i < S; ++i)
        for (int j = 0; j < S; ++j)
            dpCur[i*S + j] = 1;
    /* --- first two terms of the sequence --- */
    seq_arr[0] = (seq_t)S;                         /* a(1) */
    seq_arr[1] = (seq_t)S * (seq_t)S;              /* a(2) */
    seq_len = 2;
    initialized = 1;
}
