/*
 * A071349  Numbers k for which the GCD of the k-th primorial number and its
 *          totient (A058250) sets record.
 * https://oeis.org/A071349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A071349.  The sequence a(n) is the index k of the
 * k‑th primorial number for which gcd(Pk,φ(Pk)) sets a new record.
 * The algorithm keeps a sieve of primes, a flag array that marks
 * primes which have previously appeared as a divisor of some
 * (pk – 1), and records the prime indices where a new prime becomes
 * active.  The first record is at k=1, corresponding to the primorial
 * 2.  Each time a new prime is activated the gcd value increases,
 * thus producing a record.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* -----------------------------------------------------------------
 *  Static data and helper declarations
 * ----------------------------------------------------------------- */
static bool initialized = false;

/* Prime table ---------------------------------------------------- */
static uint64_t *primes     = NULL;   /* array of primes up to LIMIT */
static size_t   primes_cnt  = 0;      /* number of primes            */
static int     *idx_of_prime = NULL; /* prime value -> its index    */

/* Activation status ----------------------------------------------*/
static bool *active = NULL;           /* active[prime index]         */

/* Record storage -------------------------------------------------*/
static seq_t *records    = NULL;      /* list of record indices (k) */
static size_t record_cnt = 0;         /* number of records stored   */
static size_t record_cap = 0;         /* allocated capacity         */

/* Processing pointer ---------------------------------------------*/
static size_t next_pidx = 0;          /* next prime index to process */

/* -----------------------------------------------------------------
 *  Utility functions
 * ----------------------------------------------------------------- */
static void add_record(size_t k)
{
    if (record_cnt == record_cap) {
        record_cap = (record_cap == 0) ? 64 : record_cap << 1;
        records = (seq_t *)realloc(records, record_cap * sizeof(seq_t));
    }
    records[record_cnt++] = (seq_t)(k);
}

/* Process a single prime with index pi (0‑based).  If the prime
 * activates any new smaller prime divisor, a record at k = pi+1 is
 * created. */
static void process_prime(size_t pi)
{
    uint64_t p   = primes[pi];
    uint64_t r   = p - 1;          /* factorize p‑1 */
    bool new_act = false;

    for (size_t j = 0; j < primes_cnt && primes[j] * primes[j] <= r; ++j) {
        uint64_t q = primes[j];
        if (r % q == 0) {
            size_t qi = (size_t)idx_of_prime[q];
            if (!active[qi]) {
                active[qi] = true;
                new_act    = true;
            }
            while (r % q == 0) r /= q;
        }
    }
    if (r > 1) { /* remaining prime factor > sqrt */
        size_t qi = (size_t)idx_of_prime[r];
        if (!active[qi]) {
            active[qi] = true;
            new_act    = true;
        }
    }
    if (new_act)
        add_record(pi + 1);   /* k = pi+1 (1‑based index) */
}

/* Build primes up to LIMIT using a simple sieve.  The LIMIT must
 * exceed the largest prime required for the requested n.  Here we
 * choose a fixed safe limit of one million. */
static void build_primes(void)
{
    const size_t LIMIT_PRIMES = 1000000;   /* sufficient for n ≤ 10000 */
    uint8_t *is_prime = (uint8_t *)malloc((LIMIT_PRIMES + 1) * sizeof(uint8_t));
    if (!is_prime) return;                /* out of memory */

    for (size_t i = 0; i <= LIMIT_PRIMES; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (size_t i = 2; i * i <= LIMIT_PRIMES; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= LIMIT_PRIMES; j += i)
                is_prime[j] = 0;

    /* Count primes */
    primes_cnt = 0;
    for (size_t i = 2; i <= LIMIT_PRIMES; ++i)
        if (is_prime[i]) primes_cnt++;

    /* Allocate primes array */
    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT_PRIMES; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);

    /* Skip the rest of the large array after the last prime */
    size_t last_prime = primes[primes_cnt - 1];

    /* prime value -> index map */
    idx_of_prime = (int *)malloc((last_prime + 1) * sizeof(int));
    for (size_t i = 0; i <= last_prime; ++i) idx_of_prime[i] = -1;
    for (size_t i = 0; i < primes_cnt; ++i)
        idx_of_prime[primes[i]] = (int)i;

    /* Activation flags */
    active = (bool *)calloc(primes_cnt, sizeof(bool));
}

/* Initialise static data structures on first use */
static void init_sequence(void)
{
    build_primes();
    record_cap = 64;
    records = (seq_t *)malloc(record_cap * sizeof(seq_t));
    add_record(1);          /* a(1) = 1 (k = 1) */
    next_pidx = 1;          /* next prime index to process (index 1 → prime 3) */
    initialized = true;
}

/* -----------------------------------------------------------------
 *  Public function
 * ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();

    /* Ensure we have at least n records */
    while (record_cnt < (size_t)n) {
        if (next_pidx >= primes_cnt) {
            /* Should not happen for n ≤ 10000 with the chosen limit */
            return (seq_t)0;
        }
        process_prime(next_pidx);
        ++next_pidx;
    }
    return records[n - 1];
}
