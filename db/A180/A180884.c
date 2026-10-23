/*
 * A180884  Number of permutations p() of 1..n+7 with centered difference
 *          p(i+1)-p(i-1) < 0 exactly 6 times.
 * https://oeis.org/A180884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..6} C(n+7, ceil((n+7)/2)) *
 *            Eulerian(ceil((n+7)/2), i) * Eulerian(floor((n+7)/2), 6-i)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Two helper functions.  The first computes a binomial
   coefficient C(n,k) as a big integer.  The second
   makes sure the Eulerian numbers A(m,r) for 0≤r≤6
   up to a given maximum m have been pre‑computed. */

#include <stdint.h>
#include <stddef.h>

#define MAX_M 10010   /* largest odd/even length needed for n≈10000 */

static seq_t eulerian[7][MAX_M];
static int eulerian_init = 0;

static seq_t binom_uint64(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

static void ensure_eulerian(idx_t up_to)
{
    if (up_to >= MAX_M) up_to = MAX_M - 1;
    if (eulerian_init && up_to < MAX_M) return;

    /* initialize base case */
    for (int r = 0; r <= 6; ++r)
        for (idx_t m = 0; m < MAX_M; ++m)
            eulerian[r][m] = 0;
    eulerian[0][1] = 1;

    /* recurrence */
    for (idx_t m = 2; m < MAX_M; ++m) {
        eulerian[0][m] = eulerian[0][m - 1];
        for (int r = 1; r <= 6; ++r) {
            if (r >= (int)m) {
                eulerian[r][m] = 0;
            } else {
                eulerian[r][m] =
                    ((seq_t)(r + 1)) * eulerian[r][m - 1] +
                    ((seq_t)(m - r)) * eulerian[r - 1][m - 1];
            }
        }
    }
    eulerian_init = 1;
}

seq_t a(idx_t n)
{
    /* Pre‑compute Eulerian numbers up to the required size. */
    idx_t N = n + 7;
    idx_t m_odd  = (N + 1) / 2;        /* ceil((n+7)/2) */
    idx_t m_even = N / 2;              /* floor((n+7)/2) */
    ensure_eulerian((idx_t)(m_odd > m_even ? m_odd : m_even));

    seq_t bin = binom_uint64(N, m_odd);
    seq_t result = 0;

    for (int i = 0; i <= 6; ++i) {
        if (i > (int)m_odd - 1) continue;
        int j = 6 - i;
        if (j > (int)m_even - 1) continue;
        seq_t term = bin *
                     eulerian[i][m_odd] *
                     eulerian[j][m_even];
        result += term;
    }
    return result;
}
