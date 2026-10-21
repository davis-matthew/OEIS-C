/*
 * A088610  Starting with n = 1, a(n) is the smallest squarefree number not
 *          included earlier if n is odd, else n is the smallest nonsquarefree
 *          number.
 * https://oeis.org/A088610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n-1) = A005117(n) (the n‑th squarefree number), a(2n) =
 *            A013929(n) (the n‑th nonsquarefree number)
 * Range:     u8 n <= 197, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds two ascending lists:
        SF[] – squarefree numbers,
        NS[] – nonsquarefree numbers,
   then returns SF[(n+1)/2] for odd n and NS[n/2] for even n. */

#include <stdbool.h>

#define MAX_COUNT   200000   /* maximum numbers needed in each list (safety margin) */
#define PRIME_LIMIT 50000   /* limit for prime precomputation */

/* Forward declaration of the sequence function */
seq_t a(idx_t n);

/* ---------- Prime sieve (used for squarefree test) ---------- */
static bool primes_initialized = false;
static idx_t primes[PRIME_LIMIT];
static idx_t prime_count = 0;

static void init_primes(void)
{
    /* Simple trial division sieve up to PRIME_LIMIT */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        bool is_prime = true;
        for (idx_t j = 0; j < prime_count; ++j) {
            idx_t p = primes[j];
            if ((idx_t)p * (idx_t)p > i) break;
            if (i % p == 0) { is_prime = false; break; }
        }
        if (is_prime) primes[prime_count++] = i;
    }
    primes_initialized = true;
}

/* ---------- Sequence storage ---------- */
static seq_t SF[MAX_COUNT];
static seq_t NS[MAX_COUNT];
static idx_t sf_used = 0;   /* number of squarefree numbers stored */
static idx_t ns_used = 0;   /* number of nonsquarefree numbers stored */
static idx_t next_candidate = 1;   /* next integer to test for inclusion */

/* ---------- Squarefree test ---------- */
static bool is_squarefree(seq_t x)
{
    for (idx_t j = 0; j < prime_count; ++j) {
        idx_t p = primes[j];
        seq_t p2 = (seq_t)p * (seq_t)p;
        if (p2 > x) break;
        if (x % p2 == 0) return false;
    }
    return true;
}

/* ---------- Ensure lists are long enough ---------- */
static void ensure_lists(idx_t need_sf, idx_t need_ns)
{
    if (!primes_initialized) init_primes();

    while (sf_used < need_sf || ns_used < need_ns) {
        seq_t x = (seq_t)next_candidate++;
        if (is_squarefree(x))
            SF[sf_used++] = x;
        else
            NS[ns_used++] = x;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n % 2 == 1) {                 /* odd n → squarefree */
        idx_t k = (n + 1) / 2;
        ensure_lists(k, 0);
        return SF[k - 1];
    } else {                           /* even n → nonsquarefree */
        idx_t k = n / 2;
        ensure_lists(0, k);
        return NS[k - 1];
    }
}
