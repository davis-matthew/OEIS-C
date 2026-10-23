/*
 * A038876  Primes p such that 6 is a square mod p.
 * https://oeis.org/A038876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes p such that 6 is a quadratic residue modulo p (OEIS A038876).
   For p > 6 these are the primes with p ≡ 1,5,19,23 (mod 24).  The
   function generates primes incrementally and stores the required ones
   in a static table so that queries in any order are answered quickly. */
#include <stdint.h>
#include <stdlib.h>

/* Ensure that an array of element size 'elem_size' has capacity at least
   'needed'.  It reallocates the array if necessary.  The array pointer
   should be of type 'void **' so that it can be cast appropriately. */
static void _ensure_capacity(size_t needed, size_t *cap,
                             void **ptr, size_t elem_size) {
    if (needed > *cap) {
        size_t new_cap = (*cap) ? (*cap << 1) : 16;
        while (new_cap < needed) new_cap <<= 1;
        void *new_ptr = realloc(*ptr, new_cap * elem_size);
        if (!new_ptr) abort();   /* allocation failed */
        *ptr = new_ptr;
        *cap = new_cap;
    }
}

/* Integer square‐root for 64‑bit values, no floating point used. */
static uint64_t _isqrt_u64(uint64_t n) {
    uint64_t low = 0, high = 65536; /* 2^16 > sqrt(UINT64_MAX) */
    while (high * high <= n) high <<= 1;     /* find an upper bound */
    low = high >> 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

/* Current list of all primes found so far (stored as uint64_t). */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

/* Current list of special primes (those for which 6 is a square mod p). */
static seq_t *special = NULL;
static size_t special_cnt = 0, special_cap = 0;

/* Populate the initial primes list (2, 3, 5) and the special list. */
static void _initialize(void) {
    if (special_cnt) return;     /* already initialized */
    _ensure_capacity(3, &primes_cap, (void **)&primes, sizeof(uint64_t));
    primes[0] = 2; primes[1] = 3; primes[2] = 5;
    primes_cnt = 3;

    _ensure_capacity(3, &special_cap, (void **)&special, sizeof(seq_t));
    special[0] = (seq_t)2;
    special[1] = (seq_t)3;
    special[2] = (seq_t)5;
    special_cnt = 3;
}

/* Return true if candidate is prime, using the primes list for trial division.
   candidate is assumed to be odd and > 5. */
static int _is_prime(uint64_t candidate) {
    if (candidate <= 5)
        return candidate == 2 || candidate == 3 || candidate == 5;
    uint64_t limit = _isqrt_u64(candidate);
    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i) {
        if (candidate % primes[i] == 0) return 0;
    }
    return 1;
}

/* Generate primes and special primes until we have at least n special primes. */
static void _ensure_special(idx_t n) {
    _initialize();
    if (special_cnt >= (size_t)n) return;

    uint64_t candidate = primes[primes_cnt - 1] + 2;  /* next odd candidate */
    while (special_cnt < (size_t)n) {
        if (_is_prime(candidate)) {
            /* Store new prime */
            _ensure_capacity(primes_cnt + 1, &primes_cap,
                             (void **)&primes, sizeof(uint64_t));
            primes[primes_cnt++] = candidate;

            /* Check if it is a special prime (6 quadratic residue) */
            uint64_t r = candidate % 24;
            if (r == 1 || r == 5 || r == 19 || r == 23) {
                _ensure_capacity(special_cnt + 1, &special_cap,
                                 (void **)&special, sizeof(seq_t));
                special[special_cnt++] = (seq_t)candidate;
            }
        }
        candidate += 2;  /* only odd numbers need checking */
    }
}

/* The sequence function requested. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* guard against 0, although offset is 1 */
    _ensure_special((size_t)n);
    return special[n - 1];
}
