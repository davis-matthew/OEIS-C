/*
 * A093513  Transform of the prime sequence by the Rule89 cellular automaton.
 * https://oeis.org/A093513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=4; for n>3, a(n) is the (n‑4)th positive
 *            integer greater than 1 that is neither prime nor one more than a
 *            prime (i.e., a composite number whose predecessor is composite)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Limit for primality sieve – large enough to contain at least 10000
   qualifying numbers for n ≤ 10000. */
#define PRIME_LIMIT 500000
/* Capacity of the pre‑computed list of missing numbers. */
#define MISSING_CAP 20000

/* Prime flags: prime_flag[i] is true iff i is prime. */
static bool prime_flag[PRIME_LIMIT + 1];
/* List of numbers that are neither prime nor one more than a prime. */
static seq_t missing[MISSING_CAP];
static size_t missing_cnt = 0;
/* Flag indicating whether the tables have been initialized. */
static bool tables_initialized = false;

/*---------------------------------------------------------------*/
/* Initialize prime sieve and the missing‑number table.          */
/*---------------------------------------------------------------*/
static void init_tables(void)
{
    if (tables_initialized) return;

    /* Sieve of Eratosthenes. */
    for (int i = 0; i <= PRIME_LIMIT; ++i) prime_flag[i] = true;
    prime_flag[0] = prime_flag[1] = false;
    for (int i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (prime_flag[i]) {
            for (int j = i * i; j <= PRIME_LIMIT; j += i) prime_flag[j] = false;
        }
    }

    /* Build the list of qualifying numbers. */
    for (int i = 2; i <= PRIME_LIMIT && missing_cnt < MISSING_CAP; ++i) {
        bool is_prime = prime_flag[i];
        bool prev_prime = (i > 1) ? prime_flag[i - 1] : false;
        if (!is_prime && !prev_prime) {
            missing[missing_cnt++] = (seq_t)i;
        }
    }

    tables_initialized = true;
}

/*---------------------------------------------------------------*/
/* Main sequence function.                                       */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init_tables();

    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;

    size_t idx = (size_t)(n - 4);   /* index in missing[] */
    if (idx < missing_cnt) {
        return missing[idx];
    }

    /* The following should not be reached for n ≤ 10000.
       If it happens, return 0 as a safe placeholder. */
    return (seq_t)0;
}
