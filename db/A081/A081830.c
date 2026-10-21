/*
 * A081830  Numbers k such that the k-th term of Hofstadter Q-sequence is <
 *          (k+1)-th term.
 * https://oeis.org/A081830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A081830
 *  a(n) = the n-th integer k such that the k-th term of the Hofstadter
 *  Q‑sequence is strictly smaller than the (k+1)-th term.
 *
 *  The Hofstadter Q‑sequence is defined by:
 *      Q(1) = Q(2) = 1
 *      Q(n) = Q(n – Q(n‑1)) + Q(n – Q(n‑2))   for n > 2
 *
 *  The implementation evaluates Q(k) and Q(k+1) sequentially, adding k to
 *  the result list whenever Q(k) < Q(k+1).  The array of Q values and the
 *  list of qualifying k’s are statically grown so that the function can
 *  be called in any order and will retain previously computed data.
 *
 *  Time complexity is O(K) where K is the maximum k needed for the
 *  requested n; for n up to 10 000, K ≈ 2 n ≈ 20 000, which is easily
 *  within the 2‑second limit.  All arithmetic uses the generic type
 *  seq_t, which is a (signed or unsigned) integer type supplied by the
 *  build system.
 */

#include <stdlib.h>
#include <string.h>

static idx_t max_k = 0;              /* largest index of Q that has been computed   */
static seq_t *Q = NULL;              /* array of Hofstadter Q values, 1‑based       */
static size_t Qsize = 0;             /* current capacity of Q array                */

static idx_t a_len = 0;              /* number of a(k) values that have been found   */
static seq_t *A = NULL;              /* array of qualifying k values, 0‑based      */
static size_t Asize = 0;             /* current capacity of A array                */

static idx_t next_test_k = 1;        /* next k to test against Q(k+1)              */

/* Grow the Q array to hold at least 'need' indices (1‑based).  */
static void expand_Q(size_t need)
{
    if (need <= Qsize)
        return;
    size_t newsize = Qsize ? Qsize << 1 : 64;
    if (newsize < need)
        newsize = need;
    seq_t *newQ = (seq_t *)realloc(Q, newsize * sizeof(seq_t));
    if (!newQ) abort();
    memset(newQ + Qsize, 0, (newsize - Qsize) * sizeof(seq_t));
    Q = newQ;
    Qsize = newsize;
}

/* Compute the Hofstadter Q value for index 'idx'; assumes all earlier
 * values are already known.  */
static void compute_Q_next(void)
{
    ++max_k;
    expand_Q(max_k + 1);
    if (max_k == 1 || max_k == 2) {
        Q[max_k] = 1;
    } else {
        seq_t v1 = Q[max_k - (idx_t)Q[max_k - 1]];
        seq_t v2 = Q[max_k - (idx_t)Q[max_k - 2]];
        Q[max_k] = v1 + v2;
    }
}

/* Ensure that Q[1 .. idx] have been computed.  */
static void ensure_Q_up_to(idx_t idx)
{
    while (max_k < idx)
        compute_Q_next();
}

/* Grow the array holding the a(k) values to at least 'need' elements.  */
static void expand_A(size_t need)
{
    if (need <= Asize)
        return;
    size_t newsize = Asize ? Asize << 1 : 64;
    if (newsize < need)
        newsize = need;
    seq_t *newA = (seq_t *)realloc(A, newsize * sizeof(seq_t));
    if (!newA) abort();
    memset(newA + Asize, 0, (newsize - Asize) * sizeof(seq_t));
    A = newA;
    Asize = newsize;
}

/* Main entry: return the n‑th element of A081830. */
seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1; undefined for n=0; return 0 safely */
        return 0;

    /* Ensure we have computed enough a(k) values. */
    while (a_len < n) {
        /* We need Q(k) and Q(k+1); ensure they are available. */
        ensure_Q_up_to(next_test_k + 1);

        /* Does this k satisfy Q(k) < Q(k+1)? */
        if (Q[next_test_k] < Q[next_test_k + 1]) {
            expand_A(a_len + 1);
            A[a_len++] = (seq_t)next_test_k;   /* store k as seq_t */
        }
        ++next_test_k;
    }

    return A[n - 1];
}
