/*
 * A080087  Number of factors of 5 in the factorial of the n-th prime, counted
 *          with multiplicity.
 * https://oeis.org/A080087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{k\ge1} \left\lfloor \frac{p_n}{5^k}\right\rfloor
 *            where p_n is the n‑th prime. (v_5((p_n)!))
 * Range:     u8 n <= 54, u16 n <= 6676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

/* Compute the n-th prime and store primes in a static cache. */
static void ensure_primes(idx_t n)
{
    static uint32_t *primes = NULL;
    static size_t prime_count = 0;
    static uint32_t last_checked = 1;

    if (prime_count >= (size_t)n)
        return;

    /* Allocate initial space if needed. */
    if (primes == NULL) {
        size_t cap = 1000;
        primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
        if (!primes) exit(1);
        prime_count = 0;
        last_checked = 1;
    }
    while (prime_count < (size_t)n) {
        if (last_checked < 2) last_checked = 2;
        /* Test primality of last_checked. */
        uint32_t x = last_checked;
        uint32_t r = (uint32_t)sqrt((double)x);
        int is_prime = 1;
        for (size_t i = 0; i < prime_count && primes[i] <= r; ++i) {
            if (x % primes[i] == 0) { is_prime = 0; break; }
        }
        if (is_prime) {
            if (prime_count == 0 || sizeof(primes[0]) > 0) {
                /* Ensure capacity. */
                if (prime_count == 0 || prime_count % prime_count == 0) {
                    size_t new_cap = (prime_count == 0) ? 1000 : prime_count * 2;
                    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                    if (!tmp) exit(1);
                    primes = tmp;
                }
                primes[prime_count++] = x;
            }
        }
        ++last_checked;
    }
}

/* Return the a(n) value. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Offset starts at 1; behaviour for n<1 undefined. */

    ensure_primes(n);
    extern uint32_t *primes;  /* Use the same static array from ensure_primes. */
    /* Due to static block, we can reference primes directly. */
    static uint32_t *cached_primes = NULL;
    static size_t cached_count = 0;
    static uint32_t cached_last_checked = 1;
    /* The static arrays are shared; we peel them out via extern. */
    /* Actually, because we defined primes static inside ensure_primes, we can't access directly.
       Instead, we can add a helper to retrieve prime value. */
    /* For simplicity, reimplement small prime extraction here: */
    seq_t p_val;
    /* Find the n-th prime from the stored array in ensure_primes. */
    /* We'll fetch it by calling ensure_primes again, which guarantees the array exists. */
    /* Since we cannot access the static array directly, we replicate storage using a global. */

    /* We modify: define static at file scope: */
    /* --> We place the array at file scope (outside function) to store primes. */
    /* To avoid confusion, we adjust the implementation: */
#if 0
    /* Placeholder to satisfy compiler that primes is defined. */
    (void)p_val;
#endif
    /* Since the above approach is messy, we rewrite pragmatically: */

    /* Revised implementation: maintain primes in a global static buffer. */
    static uint32_t *global_primes = NULL;
    static size_t global_count = 0;
    static uint32_t global_last_checked = 1;

    /* Ensure sufficient primes. */
    if (global_count < (size_t)n) {
        if (global_primes == NULL) {
            size_t cap = 1000;
            global_primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
            if (!global_primes) exit(1);
            global_count = 0;
            global_last_checked = 1;
        }
        while (global_count < (size_t)n) {
            if (global_last_checked < 2) global_last_checked = 2;
            uint32_t x = global_last_checked;
            uint32_t r = (uint32_t)sqrt((double)x);
            int is_prime = 1;
            for (size_t i = 0; i < global_count && global_primes[i] <= r; ++i) {
                if (x % global_primes[i] == 0) { is_prime = 0; break; }
            }
            if (is_prime) {
                if (global_count == 0 || global_count % global_count == 0) {
                    size_t new_cap = (global_count == 0) ? 1000 : global_count * 2;
                    uint32_t *tmp = (uint32_t *)realloc(global_primes, new_cap * sizeof(uint32_t));
                    if (!tmp) exit(1);
                    global_primes = tmp;
                }
                global_primes[global_count++] = x;
            }
            ++global_last_checked;
        }
    }

    /* Retrieve the n-th prime. */
    seq_t prime = (seq_t)global_primes[n - 1];

    /* Legendre's formula for exponent of 5 in prime! */
    seq_t result = 0;
    seq_t temp = prime;
    while (temp >= (seq_t)5) {
        temp /= (seq_t)5;
        result += temp;
    }
    return result;
}
