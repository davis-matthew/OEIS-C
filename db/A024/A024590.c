/*
 * A024590  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (composite numbers), t = (odd natural numbers).
 * https://oeis.org/A024590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{1<=i<=floor((n+1)/2)} A002808(i) * (1+2*(n-i)) where
 *            A002808(i) is the i-th composite number (4, 6, 8, 9, 10, …)
 * Range:     u8 n <= 7, u16 n <= 60, u32 n <= 2771, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static storage for the list of composite numbers */
static uint64_t *composites      = NULL; /* array of composite numbers */
static size_t   composite_count = 0;    /* number of composites stored */
static size_t   composite_limit  = 0;    /* maximum value sieved */

/* Ensure that at least 'need' composite numbers are available. */
static void ensure_composites(size_t need)
{
    if (need <= composite_count)
        return;                          /* already sufficient */

    /* Start with a reasonable limit and double it until we have enough. */
    size_t limit = (composite_limit == 0) ? 100 : composite_limit * 2;

    while (1) {
        /* Allocate a boolean array for the sieve (1 = prime, 0 = composite) */
        uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
        if (!is_prime)
            abort();                     /* allocation failure */

        /* Initialise all entries to 1 (prime) */
        for (size_t i = 0; i <= limit; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;   /* 0 and 1 are not prime */

        /* Sieve of Eratosthenes */
        for (size_t p = 2; p * p <= limit; ++p)
            if (is_prime[p])
                for (size_t m = p * p; m <= limit; m += p)
                    is_prime[m] = 0;

        /* Count composites in this interval */
        size_t cnt = 0;
        for (size_t num = 4; num <= limit; ++num)
            if (!is_prime[num])
                ++cnt;

        if (cnt >= need) {
            /* Allocate and fill the composite array */
            uint64_t *new_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
            if (!new_arr)
                abort();

            size_t idx = 0;
            for (size_t num = 4; num <= limit; ++num)
                if (!is_prime[num])
                    new_arr[idx++] = num;

            /* Replace old storage */
            free(composites);
            composites = new_arr;
            composite_count = cnt;
            composite_limit = limit;

            free(is_prime);
            break;                       /* enough composites obtained */
        }

        /* Not enough, try a larger limit */
        free(is_prime);
        limit <<= 1;                      /* multiply by 2 */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* For n < 1, the behavior is undefined per the problem statement; */
    /* but we handle it safely by returning 0. */
    if (n < 1)
        return 0;

    /* Determine the number of terms in the sum: k = floor((n+1)/2) */
    size_t k = ((size_t)n + 1) / 2;

    /* Make sure we have at least k composite numbers */
    ensure_composites(k);

    seq_t result = 0;
    for (size_t i = 1; i <= k; ++i) {
        /* term = 1 + 2 * (n - i) */
        seq_t term = 1 + 2 * ((seq_t)n - (seq_t)i);
        result += (seq_t)composites[i - 1] * term;
    }
    return result;
}
