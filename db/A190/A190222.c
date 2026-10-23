/*
 * A190222  Noncomposite numbers all of whose decimal digits are noncomposite
 *          numbers (1,2,3,5,7).
 * https://oeis.org/A190222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 519, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A190222: Noncomposite numbers all of whose decimal digits are noncomposite numbers (1,2,3,5,7).
   The sequence consists of 1 and all primes with only digits from {1,2,3,5,7}.
   This implementation iterates integers, tests the digit constraint, and checks
   primality by trial division using a precomputed list of small primes. */

#include <stdbool.h>
#include <stdlib.h>

/* ----- Prime precomputation ----- */
static bool *prime_flag = NULL;
static unsigned *prime_list = NULL;
static size_t prime_count = 0;

static void init_primes(void) {
    if (prime_flag != NULL)
        return; /* already initialized */

    const idx_t LIMIT = 100000; /* sufficient for sqrt of required candidates */
    prime_flag = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!prime_flag)
        return; /* allocation failure, but unlikely for such a small array */

    for (idx_t i = 0; i <= LIMIT; ++i)
        prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;

    for (idx_t p = 2; p * p <= LIMIT; ++p) {
        if (prime_flag[p]) {
            for (idx_t m = p * p; m <= LIMIT; m += p)
                prime_flag[m] = false;
        }
    }

    prime_list = (unsigned *)malloc(LIMIT * sizeof(unsigned));
    if (!prime_list)
        return; /* allocation failure */

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (prime_flag[i])
            prime_list[prime_count++] = (unsigned)i;
    }
}

/* ----- Helpers ----- */
static inline bool digits_ok(idx_t x) {
    if (x == 0)
        return false;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d != 1 && d != 2 && d != 3 && d != 5 && d != 7)
            return false;
        x /= 10;
    }
    return true;
}

static bool is_prime_candidate(idx_t x) {
    if (x < 2)
        return false;
    for (size_t i = 0; i < prime_count; ++i) {
        unsigned p = prime_list[i];
        idx_t pp = (idx_t)p * (idx_t)p;
        if (pp > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n) {
    if (n < 1)
        return (seq_t)0; /* undefined for n < 1 */

    init_primes();

    idx_t count = 0;
    idx_t candidate = 1;
    for (;;) {
        if (!digits_ok(candidate))
            { ++candidate; continue; }

        if (candidate == 1 || is_prime_candidate(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
