/*
 * A120208  a(1)=7; a(n)=floor((69+sum(a(1) to a(n-1)))/9).
 * https://oeis.org/A120208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = ⌊(69 + Σ_{k=1}^{n-1} a(k)) / 9⌋
 * Range:     u8 n <= 16, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a running sum and a static cache of
   computed terms. It is suitable for any integer type defined
   as seq_t and idx_t by the build environment. The cache size
   covers the typical test range (up to 10 000). */
seq_t a(idx_t n)
{
    /* a(0) is undefined for the OEIS sequence; return 0 for convenience. */
    if (n == 0)
        return 0;

    /* static state: cache, cumulative sum, and largest n already computed */
    static idx_t  max_n = 0;                     /* largest index completed   */
    static seq_t  sum   = 0;                     /* Σ_{k=1}^{max_n} a(k)      */
    static seq_t  cache[10001];                  /* 1‑based cache (up to 10 000) */

    /* Compute terms up to the requested index. */
    while (max_n < n) {
        seq_t next = ((seq_t)69 + sum) / 9;      /* recurrence */
        ++max_n;
        cache[max_n] = next;
        sum += next;
    }

    /* Return the desired term. */
    return cache[n];
}
