/*
 * A198535  Number of 2n X 2 0..4 arrays with values 0..4 introduced in row
 *          major order and each element equal to an even number of horizontal
 *          and vertical neighbors.
 * https://oeis.org/A198535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=5, a(2)=404, a(3)=67442, a(4)=11739910, a(5)=2048681430,
 *            a(6)=357559607382 and for n>6:
 *            a(n)=189*a(n-1)-2572*a(n-2)+8272*a(n-3)-9984*a(n-4)+4096*a(n-5)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  a(0) is undefined; the function returns 0 for convenience. */

static const seq_t _init_vals[6] = {
    (seq_t)5ULL,
    (seq_t)404ULL,
    (seq_t)67442ULL,
    (seq_t)11739910ULL,
    (seq_t)2048681430ULL,
    (seq_t)357559607382ULL
};

/* Static cache of computed terms: indices are 1‑based (cache[1] == a(1)) */
static seq_t _cache[10001];
static idx_t _computed = 0;           /* largest index already computed */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* offset starts at 1 */

    if (n <= 6)
        return _init_vals[n-1];       /* direct return for the first six terms */

    /* initialise the cache with the known initial values if not already done */
    if (_computed < 6) {
        for (idx_t i = 1; i <= 6; ++i)
            _cache[i] = _init_vals[i-1];
        _computed = 6;
    }

    /* compute further terms up to n */
    for (idx_t i = _computed+1; i <= n; ++i) {
        seq_t term = _cache[i-1] * (seq_t)189
                   - _cache[i-2] * (seq_t)2572
                   + _cache[i-3] * (seq_t)8272
                   - _cache[i-4] * (seq_t)9984
                   + _cache[i-5] * (seq_t)4096;
        _cache[i] = term;
    }
    _computed = n;

    return _cache[n];
}
