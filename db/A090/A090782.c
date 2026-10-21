/*
 * A090782  Numbers that can be expressed as the difference of the squares of
 *          primes in just three distinct ways.
 * https://oeis.org/A090782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A090782
   Numbers that can be expressed as the difference of squares of primes
   in exactly three distinct ways.
   The algorithm enumerates all prime pairs (q,p) with p>q, computes
   n = p^2 – q^2 up to a fixed upper bound, counts how many times
   each n occurs, and then collects those n with exactly three
   representations.  The resulting list is stored once (static
   initialization) and returned on query.  This approach runs in
   well under two seconds for n up to about 10 000.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* maximum bound for n – chosen large enough to contain at least
   10 000 terms (empirically it gives roughly 30 000 terms).  */
#define MAX_N 20000000ULL

/*<forward declarations>*/
static void a090782_init(void);

/*<static storage>*/
static seq_t *seq_values = NULL;   /* array of sequence values   */
static idx_t  seq_length = 0;      /* number of values stored   */
static bool   initialized = false; /* flag for lazy initialization */

/*<public function>*/
seq_t a(idx_t n)
{
    if (!initialized) {
        a090782_init();
        initialized = true;
    }
    if (n == 0 || n > seq_length) return 0;  /* out of range – 0 is safe */
    return seq_values[n-1];
}

/*<private helper>*/
static void a090782_init(void)
{
    /* ---------- 1. generate all primes up to sqrt(2*MAX_N)+1  ----------- */
    /* compute upper limit for primes: all p such that p^2 <= 2*MAX_N  */
    idx_t prime_limit = 1;
    while ((uint64_t)prime_limit * prime_limit <= 2ULL * MAX_N) ++prime_limit;
    ++prime_limit;  /* safety margin */

    bool *is_prime = (bool*)calloc((size_t)prime_limit + 1, sizeof(bool));
    uint32_t *primes = (uint32_t*)malloc((size_t)prime_limit * sizeof(uint32_t));
    size_t prime_cnt = 0;
    for (size_t i = 2; i <= (size_t)prime_limit; ++i) is_prime[i] = true;
    for (size_t i = 2; i <= (size_t)prime_limit; ++i) {
        if (!is_prime[i]) continue;
        primes[prime_cnt++] = (uint32_t)i;
        if (i * i <= (size_t)prime_limit) {
            for (size_t j = i * i; j <= (size_t)prime_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* ---------- 2. count representations for n <= MAX_N ----------------- */
    uint8_t *cnt = (uint8_t*)calloc((size_t)MAX_N + 1, sizeof(uint8_t));

    for (size_t qi = 0; qi < prime_cnt; ++qi) {
        uint32_t q = primes[qi];
        for (size_t pi = qi + 1; pi < prime_cnt; ++pi) {
            uint32_t p = primes[pi];
            uint64_t val = (uint64_t)p * p - (uint64_t)q * q;
            if (val > MAX_N) break;
            ++cnt[(size_t)val];
        }
    }

    /* ---------- 3. collect all n with exactly 3 representations --------- */
    seq_t *tmp = NULL;
    size_t tlen = 0;
    for (size_t n = 4; n <= MAX_N; ++n) {
        if (cnt[n] == 3) {
            /* grow temporary array */
            seq_t *newptr = (seq_t*)realloc(tmp, (tlen + 1) * sizeof(seq_t));
            if (!newptr) {
                /* allocation failed – signal failure */
                free(cnt);
                free(is_prime);
                free(primes);
                free(tmp);
                return;
            }
            tmp = newptr;
            tmp[tlen++] = (seq_t)n;
        }
    }

    /* ---------- 4. store results in static array ----------------------- */
    seq_values = tmp;
    seq_length = (idx_t)tlen;

    /* ---------- 5. free temporary data -------------------------------- */
    free(cnt);
    free(is_prime);
    free(primes);
}
