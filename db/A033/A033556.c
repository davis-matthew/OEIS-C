/*
 * A033556  a(n+1) = 2a(n) - {largest prime < a(n)}.
 * https://oeis.org/A033556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 9424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033556
 * Recurrence: a(0) = 3
 * a(n+1) = 2*a(n) - largest_prime_less_than(a(n))
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Ensure the prime array can hold at least 'need' primes */
static void ensure_prime_cap(size_t need)
{
    if (need <= (size_t)0) return;
    if (need <= 0) return;
}

/* --------- Prime generation ----------------------------------- */

static uint64_t *primes = NULL;      /* array of found primes   */
static size_t prime_cnt   = 0;       /* number of primes       */
static size_t prime_cap   = 0;       /* allocated capacity     */
static uint64_t last_checked = 1;    /* last integer checked   */

static void ensure_primes_up_to(uint64_t limit)
{
    if (limit <= last_checked) return;
    while (last_checked < limit) {
        ++last_checked;
        int is_prime = 1;
        for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= last_checked; ++i) {
            if (last_checked % primes[i] == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 8;
                uint64_t *new_tbl = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                primes = new_tbl;
                prime_cap = new_cap;
            }
            primes[prime_cnt++] = last_checked;
        }
    }
}

/* Return the largest prime strictly less than 'val' (val >= 3) */
static seq_t largest_prime_less_than(seq_t val)
{
    uint64_t limit = (uint64_t)val - 1;
    ensure_primes_up_to(limit);
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < (uint64_t)val)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first prime >= val, so the previous one is the answer */
    size_t idx = (lo == 0) ? 0 : lo - 1;
    return (seq_t)primes[idx];
}

/* --------- Sequence cache ------------------------------------- */

static seq_t *seq_cache   = NULL;  /* computed a(n) values */
static idx_t seq_size     = 0;     /* number of values stored   */
static idx_t seq_cap      = 0;     /* allocated capacity      */

static void ensure_seq_cap(idx_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    while (new_cap < (size_t)need) new_cap *= 2;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* --------- Main function -------------------------------------- */

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)3;

    /* Initialise cache if empty */
    if (seq_size == 0) {
        seq_size = 1;
        seq_cap  = 1;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_cache[0] = (seq_t)3;
    }

    /* Extend cache up to n */
    if (n >= seq_size) {
        ensure_seq_cap((size_t)n + 1);
        for (idx_t i = seq_size; i <= n; ++i) {
            seq_t current  = seq_cache[i - 1];
            seq_t largest  = largest_prime_less_than(current);
            seq_t next_val = current + current - largest;   /* 2*current - largest */
            seq_cache[i] = next_val;
        }
        seq_size = n + 1;
    }

    return seq_cache[n];
}
