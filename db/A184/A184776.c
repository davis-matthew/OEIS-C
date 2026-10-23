/*
 * A184776  Numbers m such that prime(m) is of the form floor(k*sqrt(2));
 *          complement of A184779.
 * https://oeis.org/A184776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 178, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS sequence A184776.  The
 * sequence consists of indices m such that the m‑th prime equals
 * floor(k·√2) for some integer k.  The algorithm generates primes
 * on demand, maintains the Beatty sequence floor(k·√2) using only
 * integer arithmetic, and caches all found indices so that any
 * subsequent call is O(1).  The implementation is fully compatible
 * with C11 and C++17, uses only the approved helper functions, and
 * does not rely on any external isqrt routine. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Forward declaration of the required symbol.  No other helper functions
 * are used. */
extern seq_t a(idx_t n);

/* ---------- Integer square‑root for 64‑bit unsigned numbers ---------- */
static inline idx_t isqrt_ull(unsigned long long v)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */
    while (lo <= hi) {
        unsigned long long mid = (lo + hi) >> 1;
        unsigned long long sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi; /* floor(sqrt(v)) */
}

/* ---------- Beatty sequence generator: floor(k·√2) using integer math ----------
 *  floor(k·√2) = floor( sqrt(2·k²) ).  The right hand side is computed
 *  exactly with 128‑bit arithmetic to avoid overflow for any k that
 *  can appear for the requested n (n ≤ 10 000 easily fits into 64‑bit).
 */
static inline idx_t beatty(idx_t k)
{
    unsigned __int128 t = (unsigned __int128)k * k * 2;
    return isqrt_ull((unsigned long long)t); /* t < 2^64 for all relevant k */
}

/* ---------- Prime generator with simple trial division ----------
 *  The generator supplies successive primes.  It stores the primes
 *  it has found in a dynamic array for future divisions.
 */
static unsigned long long *prime_list = NULL;   /* primes array       */
static size_t prime_cap    = 0;                 /* capacity           */
static size_t prime_len    = 0;                 /* number of primes   */
static unsigned long long next_candidate = 3;   /* next odd to test   */

static inline unsigned long long next_prime(void)
{
    if (prime_len == 0) { /* first prime: 2 */
        if (prime_cap == 0) {
            prime_cap = 16;
            prime_list = (unsigned long long*)malloc(prime_cap * sizeof(unsigned long long));
        }
        prime_list[0] = 2;
        prime_len = 1;
        return 2;
    }

    while (1) {
        unsigned long long p = next_candidate;
        next_candidate += 2; /* skip even numbers */
        bool prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            unsigned long long d = prime_list[i];
            if ((unsigned __int128)d * d > p) break;
            if (p % d == 0) { prime = false; break; }
        }
        if (prime) {
            if (prime_len == prime_cap) {
                prime_cap <<= 1;
                prime_list = (unsigned long long*)realloc(prime_list,
                                                         prime_cap * sizeof(unsigned long long));
            }
            prime_list[prime_len++] = p;
            return p;
        }
    }
}

/* ---------- Cache of indices m that satisfy the condition ----------
 *  The cache is a dynamically resized array of seq_t values.
 */
static seq_t *good_indices = NULL;   /* indices m that satisfy the condition */
static size_t good_cap     = 0;      /* capacity of good_indices            */
static size_t good_len     = 0;      /* number of stored indices            */

/* Append a new m to the cache, expanding the array if necessary. */
static inline void push_good(idx_t m)
{
    if (good_cap == 0) {
        good_cap = 64;
        good_indices = (seq_t*)malloc(good_cap * sizeof(seq_t));
    } else if (good_len == good_cap) {
        good_cap <<= 1;
        good_indices = (seq_t*)realloc(good_indices,
                                       good_cap * sizeof(seq_t));
    }
    good_indices[good_len++] = (seq_t)m;
}

/* ---------- Beatty sequence state ----------
 *  beat_k  : current k (1‑based)
 *  beat_val: floor(k·√2)
 */
static idx_t beat_k   = 1;
static idx_t beat_val = 1; /* floor(1·√2) */

/* Advance beat_val until it is at least n.  After the call
 * beat_val == floor(k·√2) >= n for the first time. */
static inline void advance_beat_until(idx_t n)
{
    while (beat_val < n) {
        ++beat_k;
        beat_val = beatty(beat_k);
    }
}

/* ---------- Public function required by the specification ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n<1; guard anyway */

    /* Fill the cache until it contains at least n elements. */
    while (good_len < n) {
        unsigned long long p = next_prime();          /* new prime p_m          */
        idx_t m = prime_len;                         /* its 1‑based index     */

        advance_beat_until(p);                       /* ensure beat_val >= p  */
        if (beat_val == p) {
            push_good(m);                             /* store m if condition   */
        }
    }
    return good_indices[n - 1];
}
