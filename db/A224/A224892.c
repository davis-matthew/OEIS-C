/*
 * A224892  Dirichlet g.f.: Product_{k>=2} (1 - 1/k^(s-1)).
 * https://oeis.org/A224892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 127, i16 n <= 512, i32 n <= 512, i64 n <= 512,
 *            i128 n <= 512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A224892
 *
 *  Dirichlet g.f.:   product_{k>=2} (1 - k^(1-s))
 *
 *  The coefficients a(n) are given by
 *
 *      a(n) = n * Σ_{S ⊆ {d | d>1, d|n}, ∏S = n} (-1)^{|S|} ,
 *
 *  i.e. for every subset S of the proper divisors of n (including n itself)
 *  whose product equals n, add +n if |S| is even and -n if |S| is odd.
 *
 *  The implementation enumerates all such subsets by a simple depth‑first
 *  search over the list of divisors of n (sorted ascending).  For each
 *  subset the parity of its size is tracked; the total contribution of
 *  all subsets is n multiplied by the difference (even minus odd) in the
 *  number of subsets.  This works in O(number of subsets) time, which
 *  is negligible for n ≤ 10 000.
 *
 *  The routine uses only standard C11 facilities and therefore also
 *  compiles as C++17.
 */

#include <stdint.h>

/* forward declaration of helper recursive function */
static int dfs(const uint64_t *divs, int divs_size, int start,
               uint64_t rem, int parity);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined, but return 0 safely */
        return (seq_t)0;
    if (n == 1)            /* a(1) = 1 */
        return (seq_t)1;

    uint64_t N = (uint64_t)n;

    /* gather divisors > 1 of N (including N itself) */
    uint64_t divs[256];      /* n ≤ 10 000 ⇒ number of divisors < 256 */
    int divs_cnt = 0;
    for (uint64_t i = 2; i * i <= N; ++i) {
        if (N % i == 0) {
            divs[divs_cnt++] = i;
            uint64_t j = N / i;
            if (j != i)
                divs[divs_cnt++] = j;
        }
    }
    divs[divs_cnt++] = N;   /* include n itself */

    /* simple insertion sort of the divisor array */
    for (int i = 1; i < divs_cnt; ++i) {
        uint64_t key = divs[i];
        int j = i - 1;
        while (j >= 0 && divs[j] > key) {
            divs[j + 1] = divs[j];
            --j;
        }
        divs[j + 1] = key;
    }

    /* compute the parity difference via DFS */
    int diff = dfs(divs, divs_cnt, 0, N, 0);   /* parity 0 = even */

    return (seq_t)N * diff;
}

/* Recursive depth‑first search for subsets of divisors.
 * Parameters:
 *   divs      - array of sorted divisors (>1) of the original n
 *   divs_size - number of elements in divs
 *   start     - index in divs from which to try the next divisor
 *   rem       - remaining factor that should be achieved by the
 *               selected divisors so far
 *   parity    - 0 for an even number of chosen divisors, 1 for odd
 *
 * Returns the total contribution (sum of (-1)^{|S|}) for all subsets
 * with product exactly N (reached when rem == 1).
 */
static int dfs(const uint64_t *divs, int divs_size, int start,
               uint64_t rem, int parity)
{
    if (rem == 1)
        return parity ? -1 : 1;      /* base case: subset completed */

    int sum = 0;
    for (int j = start; j < divs_size; ++j) {
        uint64_t d = divs[j];
        if (d > rem)
            break;                     /* sorted order */
        if (rem % d != 0)
            continue;                  /* does not divide remainder */

        /* choose divisor d and recurse with next index and toggled parity */
        sum += dfs(divs, divs_size, j + 1, rem / d, !parity);
    }
    return sum;
}
