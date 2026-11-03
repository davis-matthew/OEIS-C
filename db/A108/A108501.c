/*
 * A108501  Number of factorizations of 4*n into even numbers.
 * https://oeis.org/A108501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of 4*n into even
 *            integers (each factor ≥ 2). The computation is performed by
 *            recursively enumerating factorizations in non‑decreasing order
 *            of factors, which avoids duplicate counts
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 113 terms through n = 9015 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t count_even_factorizations(idx_t N, idx_t min_factor)
{
    seq_t total = 0;
    for (idx_t d = min_factor; d <= N; d += 2) {      /* only even d */
        if ((N % d) != 0) continue;                  /* d must divide N   */
        if (d == N) {
            total += 1;                               /* trivial factorization N itself */
        } else {
            total += count_even_factorizations(N / d, d);
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset starts at 1 */
    seq_t N = (seq_t)n * 4;          /* 4*n, computed in seq_t to avoid overflow */
    return count_even_factorizations((idx_t)N, 2);   /* start with minimal even factor 2 */
}
