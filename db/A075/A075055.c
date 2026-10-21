/*
 * A075055  Smallest integer of the form product (n+1)(n+2)...(n+k)/n!.
 * https://oeis.org/A075055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = smallest integer of the form (n+1)(n+2)...(n+k)/n!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A075055.  The function requires no external
   libraries; it generates the necessary primes internally.  The
   algorithm factorises the numbers (n+1), (n+2), … until the
   exponents of each prime in n! are exhausted, keeping track of the
   remaining product which is the desired quotient.  It works for
   any seq_t/idx_t combination that the build supports. */

#include <stdlib.h>

/* ----- Helper: generate all primes ≤ bound and return the count. ----- */
static size_t generate_primes(idx_t bound, uint64_t *primes)
{
    if (bound < 2) return 0;  /* no primes */
    size_t i, count = 0;
    /* Simple sieve of Eratosthenes */
    char *is_prime = (char *)malloc((size_t)bound + 1);
    if (!is_prime) return 0;
    for (i = 0; i <= (size_t)bound; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= (size_t)bound; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)bound; j += i)
                is_prime[j] = 0;
    for (i = 2; i <= (size_t)bound; ++i)
        if (is_prime[i])
            primes[count++] = (uint64_t)i;
    free(is_prime);
    return count;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n <= 1) return (seq_t)1;

    /* Allocate primes up to n */
    size_t pcount = (size_t)n + 1;   /* upper bound for array size */
    uint64_t *primes = (uint64_t *)malloc(pcount * sizeof(uint64_t));
    if (!primes) return (seq_t)1;    /* fallback on allocation failure */
    size_t num_primes = generate_primes(n, primes);

    /* Remaining exponents of each prime in n! */
    int *rem = (int *)malloc(num_primes * sizeof(int));
    if (!rem) {
        free(primes);
        return (seq_t)1;
    }
    int remain_total = 0;
    for (size_t j = 0; j < num_primes; ++j) {
        uint64_t p = primes[j];
        uint64_t t = (uint64_t)n;
        int e = 0;
        while (t) { t /= p; e += (int)t; }
        rem[j] = e;
        remain_total += e;
    }

    seq_t result = (seq_t)1;
    idx_t k = 1;  /* current offset from n */

    while (remain_total > 0) {
        uint64_t val = (uint64_t)n + (uint64_t)k;
        uint64_t tmp = val;

        /* Factor using known primes up to sqrt(tmp) */
        for (size_t j = 0;
             j < num_primes && primes[j] * primes[j] <= tmp;
             ++j) {
            uint64_t p = primes[j];
            if (tmp % p != 0) continue;
            int cnt = 0;
            while (tmp % p == 0) { tmp /= p; ++cnt; }

            /* Cancel with remaining exponent if p ≤ n */
            int used = 0;
            if (p <= (uint64_t)n) {
                int need = rem[j];
                used = cnt < need ? cnt : need;
                rem[j] -= used;
                remain_total -= used;
                cnt -= used;
            }
            /* Multiply leftover power of p into the result */
            while (cnt--) result *= (seq_t)p;
        }

        /* Handle remaining prime factor after trial division */
        if (tmp > 1) {
            uint64_t p = tmp;          /* tmp is prime > sqrt(original val) */
            if (p <= (uint64_t)n) {
                /* Find index of p in prime list */
                size_t j = 0;
                while (j < num_primes && primes[j] < p) ++j;
                if (j < num_primes && primes[j] == p) {
                    if (rem[j] > 0) {
                        rem[j]--; remain_total--;
                    } else {
                        result *= (seq_t)p;
                    }
                } else {
                    /* Should not happen: prime > n but not in list */
                    result *= (seq_t)p;
                }
            } else {
                result *= (seq_t)p;
            }
        }

        ++k;
    }

    free(rem);
    free(primes);
    return result;
}
