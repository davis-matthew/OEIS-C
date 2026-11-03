/*
 * A224715  The number of unordered partitions {a,b} of prime(n) such that a
 *          or b is a nonnegative composite and the other is prime.
 * https://oeis.org/A224715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 257, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 83 terms through n = 4565 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A224715
 *  The number of unordered partitions {a,b} of prime(n) such that
 *  one part is a nonnegative composite and the other part is prime.
 *  Computation is performed via a sieve of Eratosthenes and a lookup
 *  of the n‑th prime from get_prime.h.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static unsigned char *prime_flag = NULL;   /* sieve flags */
static size_t       sieve_n   = 0;          /* size of current sieve */

/*  Ensure that prime_flag contains a sieve up to at least 'limit'.
 *  The sieve is rebuilt when a larger limit is required. */
static void ensure_sieve_upto(idx_t limit)
{
    if (prime_flag == NULL || (size_t)limit >= sieve_n) {
        size_t new_n = (size_t)limit + 1;
        unsigned char *new_flag = (unsigned char *)malloc(new_n);
        if (new_flag == NULL) exit(1);

        for (size_t i = 0; i < new_n; ++i) new_flag[i] = 1;
        if (new_n > 0) new_flag[0] = 0;
        if (new_n > 1) new_flag[1] = 0;

        size_t maxp = new_n - 1;
        for (size_t p = 2; p * p <= maxp; ++p) {
            if (new_flag[p]) {
                for (size_t m = p * p; m <= maxp; m += p)
                    new_flag[m] = 0;
            }
        }

        free(prime_flag);
        prime_flag = new_flag;
        sieve_n   = new_n;
    }
}

/*  a(idx_t n)   –   OEIS sequence A224715 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    uint32_t p = get_prime(n);           /* p = prime(n) */
    ensure_sieve_upto(p);

    seq_t cnt = 0;
    for (uint32_t a_val = 1; a_val <= p / 2; ++a_val) {
        uint32_t b_val = p - a_val;

        bool a_prime = prime_flag[a_val];
        bool b_prime = prime_flag[b_val];

        bool a_comp = (a_val > 1) && !a_prime;   /* composite (>=4) */
        bool b_comp = (b_val > 1) && !b_prime;

        if ((a_comp && b_prime) || (b_comp && a_prime))
            ++cnt;
    }
    return cnt;
}
