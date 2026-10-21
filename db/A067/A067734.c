/*
 * A067734  Number of ways writing n as a product of decimal digits of some
 *          other number which has no digits equal to 1.
 * https://oeis.org/A067734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of integers composed only of the digits 2–9 (no
 *            digit 1) whose product of digits equals n, for n ≥ 1. Recurrence
 *            (used internally): f(1) = 1 f(n) = Σ_{d∈{2,…,9}, d|n} f(n/d) (n
 *            > 1) Then a(1) = 0, and a(n) = f(n) for n > 1. The
 *            implementation memoises f(n) with an open‑addressing ha...
 * Range:     u8 n <= 287, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static const int DIGITS[8] = {2,3,4,5,6,7,8,9};

static size_t _map_cap = 0;      /* capacity (power of two), >0 after init */
static size_t _map_sz  = 0;      /* number of stored entries */
static idx_t  *_map_keys = NULL; /* keys (products) */
static seq_t *_map_vals = NULL;  /* values (f(product)) */

/* Initialise the hash map (called lazily on first use). */
static void _map_init(void)
{
    if (_map_cap != 0) return;            /* already initialised */
    _map_cap = 16;
    _map_keys = (idx_t *)malloc(_map_cap * sizeof(idx_t));
    _map_vals = (seq_t *)malloc(_map_cap * sizeof(seq_t));
    for (size_t i = 0; i < _map_cap; ++i) _map_keys[i] = 0;
}

/* Grow the table to new_cap (must be a power of two). */
static void _map_resize(size_t new_cap)
{
    idx_t *old_keys = _map_keys;
    seq_t *old_vals = _map_vals;
    size_t old_cap  = _map_cap;

    _map_keys = (idx_t *)malloc(new_cap * sizeof(idx_t));
    _map_vals = (seq_t *)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < new_cap; ++i) _map_keys[i] = 0;

    _map_cap = new_cap;
    _map_sz  = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        if (old_keys[i] == 0) continue;   /* empty slot */
        idx_t key   = old_keys[i];
        seq_t value = old_vals[i];
        size_t h = key & (new_cap - 1);
        while (_map_keys[h] != 0) h = (h + 1) & (new_cap - 1);
        _map_keys[h] = key;
        _map_vals[h] = value;
        _map_sz++;
    }

    free(old_keys);
    free(old_vals);
}

/* Store a key/value pair into the hash map. */
static void _hash_set(idx_t key, seq_t value)
{
    if (_map_cap == 0) _map_init();

    /* Resize if load factor > 3/4. */
    if (_map_sz * 4 >= _map_cap * 3) _map_resize(_map_cap * 2);

    size_t h = key & (_map_cap - 1);
    while (_map_keys[h] != 0) h = (h + 1) & (_map_cap - 1);
    _map_keys[h] = key;
    _map_vals[h] = value;
    _map_sz++;
}

/* Retrieve the value for a key; return 1 if found, 0 otherwise. */
static int _hash_get(idx_t key, seq_t *out)
{
    if (_map_cap == 0) return 0;
    size_t h = key & (_map_cap - 1);
    while (_map_keys[h] != 0) {
        if (_map_keys[h] == key) {
            *out = _map_vals[h];
            return 1;
        }
        h = (h + 1) & (_map_cap - 1);
    }
    return 0;
}

/* Recursive evaluation of f(n).  Uses memoisation. */
static seq_t _rec(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;

    seq_t cached;
    if (_hash_get(n, &cached)) return cached;

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < 8; ++i) {
        int d = DIGITS[i];
        if (n % (idx_t)d == 0) {
            sum += _rec(n / (idx_t)d);
        }
    }

    _hash_set(n, sum);
    return sum;
}

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;   /* definition of a(1) */
    return _rec(n);
}
