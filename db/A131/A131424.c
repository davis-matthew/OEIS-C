/*
 * A131424  Triangle read by rows: T(n,k) = prime(n) + prime(k) - 3, 1 <= k <=
 *          n.
 * https://oeis.org/A131424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 527, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:
 *   Sequence A131424 – triangle read by rows: T(n,k) = prime(n)+prime(k)-3,
 *   for 1 ≤ k ≤ n.  The sequence a(n) is the nth entry in the linear
 *   enumeration of that triangle.
 *
 * Formula used:
 *   a(n) = prime(r)+prime(c)-3
 *   where r is the row index such that triangular(r-1) < n ≤ triangular(r)
 *   and c = n - triangular(r-1) is the column index within that row.
 *
 * Preconditions:
 *   seq_t and idx_t are defined by the build system.
 *   The program links against the utilities listed in the problem statement.
 *
 * Complexity:
 *   O(r) to find the row, where r ≈ √(2n).  Prime generation is lazy
 *   and incremental, so each prime up to the required index is computed only
 *   once.  The implementation is fast enough for n up to at least 10 000.
 *
 * Note:
 *   The code uses malloc/realloc without free; the static cache lives
 *   until program termination, which is fine for the intended use.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ----------------------- Integer square root -------------------------- */
static inline idx_t _isqrt_u64(idx_t x) {
    if (x < 2) return x;
    idx_t lo = 1, hi = x >> 1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x)  lo = mid + 1;
        else        hi = mid - 1;
    }
    return hi;
}

/* ----------------------- Prime cache ------------------------------- */
struct {
    uint32_t *primes;        /* array of primes, 1‑based for convenience */
    size_t   cnt;            /* number of primes stored */
    size_t   cap;            /* capacity of the array */
    uint32_t next;           /* next odd candidate to test */
    bool     init;           /* true after first use */
} _prime_cache = {NULL, 0, 0, 3, false};

static uint32_t _prime_n(idx_t n) {
    if (n == 0) return 0;          /* safety fall‑back */

    if (!_prime_cache.init) {
        _prime_cache.init = true;
        /* seed with first prime 2 */
        _prime_cache.cap = 32;
        _prime_cache.primes = (uint32_t*)malloc(_prime_cache.cap * sizeof(uint32_t));
        _prime_cache.primes[0] = 2;
        _prime_cache.cnt = 1;
        _prime_cache.next = 3;
    }

    while (_prime_cache.cnt < (size_t)n) {
        uint32_t p = _prime_cache.next;
        bool is_prime = true;
        uint32_t lim = (uint32_t)_isqrt_u64(p);
        for (size_t i = 0; i < _prime_cache.cnt && _prime_cache.primes[i] <= lim; ++i) {
            if (p % _prime_cache.primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (_prime_cache.cnt == _prime_cache.cap) {
                _prime_cache.cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(_prime_cache.primes,
                                                   _prime_cache.cap * sizeof(uint32_t));
                if (!tmp) return 0;   /* unlikely, but prevents UB */
                _prime_cache.primes = tmp;
            }
            _prime_cache.primes[_prime_cache.cnt++] = p;
        }
        _prime_cache.next += 2;          /* skip even numbers */
    }
    return _prime_cache.primes[n - 1];
}

/* ------------------- Triangular number ----------------------------- */
static inline idx_t _triangular(idx_t r) {
    return r * (r + 1) / 2;
}

/* ---------------------- Main sequence ------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* out of bounds: arbitrary */

    /* find row index r: smallest r with triangular(r) ≥ n */
    idx_t r = 1;
    while (_triangular(r) < n) ++r;

    /* column index within the row */
    idx_t c = n - _triangular(r - 1);

    /* fetch the required primes and compute the value */
    seq_t pr = (seq_t)_prime_n(r);
    seq_t pc = (seq_t)_prime_n(c);
    return pr + pc - 3;
}
