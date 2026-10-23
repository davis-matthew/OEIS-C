/*
 * A171508  Numbers that are not the sum of the k-th noncomposite number and k
 *          for any k >= 1.
 * https://oeis.org/A171508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A171508 : Numbers that are not the sum of the k-th noncomposite number and k
 *  for any k >= 1.
 *
 *  Implementation notes:
 *  - Noncomposite numbers are 1 and the primes.
 *  - For k >= 2, the k-th noncomposite number equals prime_{k-1}.
 *  - Compute the set S_k = a_k + k and return the n-th integer not in this set.
 *  - The algorithm keeps a dynamic list of the excluded numbers S_k, extending
 *    it lazily on demand.  It works in O(n log n) time for n up to ~10^5
 *    and satisfies the given constraints.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime storage ---------- */
static uint64_t *primes = NULL;   /* primes[0] = 2 */
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;

static void grow_primes(void)
{
    if (prime_cnt == prime_cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        prime_cap = new_cap;
    }
}

static void add_prime(uint64_t p)
{
    grow_primes();
    primes[prime_cnt++] = p;
}

/* smallest prime >= n */
static uint64_t next_prime_ge(uint64_t n)
{
    if (n <= 2) return 2;
    uint64_t cand = (n & 1) ? n : n + 1;  /* make odd */
    while (1) {
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) { ok = false; break; }
        }
        if (ok) {
            if (cand > primes[prime_cnt - 1]) add_prime(cand);
            return cand;
        }
        cand += 2;
    }
}

/* ---------- Excluded numbers S_k = a_k + k ---------- */
static seq_t *excl = NULL;   /* sorted list of excluded numbers */
static size_t excl_cnt = 0;
static size_t excl_cap = 0;
static idx_t  next_k   = 1;   /* next k to be processed */

static void grow_excl(void)
{
    if (excl_cnt == excl_cap) {
        size_t new_cap = excl_cap ? excl_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(excl, new_cap * sizeof(seq_t));
        excl = tmp;
        excl_cap = new_cap;
    }
}

static void gen_excluded_up_to(idx_t bound)
{
    while (1) {
        idx_t k = next_k;
        seq_t a_k;
        if (k == 1) {
            a_k = (seq_t)1;
        } else {
            /* ensure prime_{k-1} is available */
            if ((idx_t)(k - 2) >= (idx_t)prime_cnt) {
                uint64_t dummy = next_prime_ge(primes[prime_cnt - 1] + 1);
                (void)dummy;  /* added inside next_prime_ge */
            }
            a_k = (seq_t)primes[k - 2];   /* primes[0] = 2 for k=2 */
        }
        seq_t s_k = a_k + (seq_t)k;
        if (s_k > (seq_t)bound) break;
        grow_excl();
        excl[excl_cnt++] = s_k;
        ++next_k;
    }
}

/* number of excluded values <= x; assumes excl sorted */
static size_t count_excl_leq(idx_t x)
{
    size_t c = 0;
    while (c < excl_cnt && excl[c] <= (seq_t)x) ++c;
    return c;
}

/* ---------- Public sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise static data on first call */
    if (!primes) {
        prime_cap = 16;
        primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        add_prime(2);                 /* start with prime 2   */
    }
    if (!excl) {
        excl_cap = 16;
        excl = (seq_t *)malloc(excl_cap * sizeof(seq_t));
        excl_cnt = 0;
        next_k = 1;
    }

    /* find a bound large enough so that "included count" >= n   */
    idx_t bound = n * 3 + 10; /* crude initial guess */
    for (;;) {
        gen_excluded_up_to(bound);
        idx_t incl = bound - (idx_t)excl_cnt;
        if (incl >= n) break;
        bound += bound / 2 + 1;
    }

    /* binary search for smallest x with incl >= n            */
    idx_t lo = 1, hi = bound;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        idx_t c_excl = (idx_t)count_excl_leq(mid);
        idx_t c_incl = mid - c_excl;
        if (c_incl >= n) hi = mid;
        else             lo = mid + 1;
    }
    return (seq_t)lo;
}
