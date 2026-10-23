/*
 * A024533  [ (4th elementary symmetric function of P(n))/(first elementary
 *          symmetric function of P(n)) ], where P(n) = {1, p(1), p(2), ...,
 *          p(n-1)}, where p(0) = 1.
 * https://oeis.org/A024533
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e4(P(n))/e1(P(n)), where P(n) = {1, p(1), p(2), … ,
 *            p(n-1)} and p(k) is the k‑th prime
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 20, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   e1 = Σx∈P(n) x
    e4 = Σ_{i<j<k<l} x_i x_j x_k x_l
   a(n) is the integer division e4/e1.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t total = 0;              /* number of elements added to P(n) so far */
static seq_t e[5] = {1, 0, 0, 0, 0}; /* elementary symmetric sums;
                                        e[0] is always 1 */

/* prime storage */
static uint64_t *prime_list = NULL;
static size_t prime_len  = 0;
static size_t prime_cap  = 0;

/* cache of a(n) values, indexed by n */
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;

static void ensure_prime_cap(size_t need) {
    if (prime_cap >= need) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
#ifdef __cplusplus
    uint64_t *p = reinterpret_cast<uint64_t *>(
        realloc(prime_list, new_cap * sizeof(uint64_t)));
#else
    uint64_t *p = realloc(prime_list, new_cap * sizeof(uint64_t));
#endif
    prime_list = p;
    prime_cap  = new_cap;
}

static void generate_next_prime(void) {
    uint64_t cand = (prime_len == 0) ? 2 : prime_list[prime_len - 1] + 1;
    for (;;) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_len; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            ensure_prime_cap(prime_len + 1);
            prime_list[prime_len++] = cand;
            break;
        }
        ++cand;
    }
}

static void ensure_a_cap(size_t need) {
    if (a_cache_cap >= need) return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 16;
    if (new_cap < need) new_cap = need;
#ifdef __cplusplus
    seq_t *p = reinterpret_cast<seq_t *>(
        realloc(a_cache, new_cap * sizeof(seq_t)));
#else
    seq_t *p = realloc(a_cache, new_cap * sizeof(seq_t));
#endif
    a_cache = p;
    a_cache_cap = new_cap;
}

/* add a new element to the elementary symmetric sums and
 * store a(n) if we now have at least 4 elements */
static void add_element(uint64_t val) {
    seq_t sv = (seq_t)val;
    for (int k = 4; k >= 1; --k) {
        e[k] = e[k] + sv * e[k - 1];
    }
    ++total;
    if (total >= 4) {
        ensure_a_cap(total + 1);
        a_cache[total] = e[4] / e[1];
    }
}

seq_t a(idx_t n) {
    if (n < 4) return 0;   /* undefined below offset */

    /* Ensure we have computed up to n */
    while (total < n) {
        if (total == 0) {           /* first element is 1 */
            add_element(1);
        } else {
            /* next prime to use is p(total) (0‑based index total-1) */
            while (prime_len <= total - 1) {
                generate_next_prime();
            }
            uint64_t p = prime_list[total - 1];
            add_element(p);
        }
    }
    return a_cache[n];
}
