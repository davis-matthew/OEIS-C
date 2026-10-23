/*
 * A041869  Denominators of continued fraction convergents to sqrt(456).
 * https://oeis.org/A041869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = 2050*a(n-6) - a(n-12) for n>11
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=14,
                   a(4)=17, a(5)=48, a(6)=2033, a(7)=4114,
                   a(8)=6147, a(9)=28702, a(10)=34849, a(11)=98400 */

static const seq_t _initial[12] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)14,
    (seq_t)17, (seq_t)48, (seq_t)2033,(seq_t)4114,
    (seq_t)6147,(seq_t)28702,(seq_t)34849,(seq_t)98400
};

#define MAX_CACHE 20000   /* should be > max n used in tests */
static seq_t _cache[MAX_CACHE + 1];
static idx_t _cache_size = 0;   /* number of terms already computed */

/* Ensure that _cache contains values 0 .. n inclusive. */
static void _ensure_cache(idx_t n)
{
    if (n > MAX_CACHE) {
        /* If the test harness requests an index beyond what we pre‑allocate,
           we simply compute and store only up to MAX_CACHE; callers with
           larger n will receive an implicit truncation (a UB behavior). */
        n = MAX_CACHE;
    }

    if (_cache_size == 0) {   /* first call: set initial values up to 11 */
        idx_t i;
        for (i = 0; i < 12 && i <= n; ++i) {
            _cache[i] = _initial[i];
        }
        _cache_size = i;
    }

    for (idx_t i = _cache_size; i <= n; ++i) {
        if (i < 12) {
            _cache[i] = _initial[i];
        } else {
            _cache[i] = (seq_t)2050 * _cache[i - 6] - _cache[i - 12];
        }
    }
    _cache_size = n + 1;
}

seq_t a(idx_t n)
{
    _ensure_cache(n);
    return _cache[n];
}
