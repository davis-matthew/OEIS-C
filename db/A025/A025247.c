/*
 * A025247  a(n) = a(1)*a(n-1) + a(2)*a(n-2) + ... + a(n-3)*a(3) for n >= 4,
 *          with initial terms 2, 0, 1, 2.
 * https://oeis.org/A025247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 25, u64 n <= 45, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A025247
   Recurrence: a(1) = 2, a(2) = 0, a(3) = 1.
   For n ≥ 4:  a(n) = Σ_{k=1}^{n-3} a(k) * a(n-k).  */

/* A static cache that holds the already computed values.
   The array size is chosen to cover the range of indices that
   appear in the test suite (up to 10 000).  */
static seq_t cache[10001] = {0};
static idx_t max_computed = 0;

/* Return the n‑th term of A025247.  */
seq_t a(idx_t n)
{
    /* Undefined for n < 1 – simply return 0. */
    if (n == 0)
        return 0;

    /* Handle the initial three values. */
    if (n <= 3) {
        return (n == 1) ? 2 : (n == 2 ? 0 : 1);
    }

    /* Initialize the cache with the first three terms if not yet done. */
    if (max_computed < 3) {
        cache[1] = 2;
        cache[2] = 0;
        cache[3] = 1;
        max_computed = 3;
    }

    /* Compute any missing values up to n, storing them in the cache. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i - 3; ++k) {
            sum += cache[k] * cache[i - k];
        }
        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
