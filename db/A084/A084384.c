/*
 * A084384  a(1) = 2; a(n+1) = smallest k > a(n) that is divisible by at most
 *          (1/2)*[tau(k)] previous terms.
 * https://oeis.org/A084384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(n+1)= smallest k > a(n) such that k is divisible by at
 *            most floor(tau(k)/2) of the previous terms a(1),...,a(n). The
 *            code builds the sequence lazily and memoizes it. It works for
 *            any seq_t type that supports +, -, *, /, %, comparison
 *            operators, and default construction
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*----------- helper for divisor count ------------------------------------*/
/*  We maintain a table of smallest prime factors for 64‑bit numbers.  This
 *  is enough for all integers that appear for n up to about 10,000.           */
static uint64_t *spf = NULL;          /* smallest prime factor table   */
static uint64_t spf_limit = 0;        /* current size of spf table     */

/* Builds a new SPF table that covers 0..limit.  The old table is freed.    */
static void build_spf_up_to(uint64_t limit)
{
    uint64_t *new_spf = (uint64_t *)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    if (!new_spf[j]) new_spf[j] = i;
            }
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Computes the number of divisors tau(n) for 64‑bit integers.               */
static uint64_t tau_of(uint64_t n)
{
    if (n > spf_limit) build_spf_up_to(n * 2);   /* extend if needed */

    uint64_t result = 1;
    while (n > 1) {
        uint64_t p = spf[n];
        uint64_t exp = 0;
        while (n % p == 0) {
            n /= p;
            ++exp;
        }
        result *= (exp + 1);
    }
    return result;
}

/*----------- sequence storage --------------------------------------------*/
static seq_t *seq  = NULL;   /* array of generated terms          */
static size_t seq_len   = 0;     /* number of terms stored            */
static size_t seq_cap   = 0;     /* allocated capacity of seq array   */

/* Initializes the sequence cache on first call.                             */
static void init_sequence(void)
{
    if (seq) return;               /* already initialized */

    seq_cap = 128;
    seq     = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 1;
    seq[0] = (seq_t)2;              /* a(1) = 2 */

    build_spf_up_to(200000);        /* provide a reasonable initial SPF table */
}

/*----------- main function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe return */

    init_sequence();

    while ((idx_t)seq_len < n) {
        /* start searching from the number right after the last term       */
        seq_t candidate = seq[seq_len - 1] + 1;
        /* find the first acceptable candidate */
        while (true) {
            /* Compute tau(candidate) using 64‑bit arithmetic; safe for the
             * range of indices we need.  If candidate exceeds 64‑bit,
             * the cast truncates; for the required limits this never
             * happens. */
            uint64_t cand_u64 = (uint64_t)candidate;
            if (cand_u64 > spf_limit) build_spf_up_to(cand_u64 * 2);
            uint64_t tau      = tau_of(cand_u64);
            uint64_t threshold = tau >> 1;          /* floor(tau/2) */

            /* Count how many previous terms divide candidate, stopping
             * early if the count would exceed the threshold. */
            uint64_t divisor_count = 0;
            for (size_t i = 0; i < seq_len; ++i) {
                if ((candidate % seq[i]) == 0) {
                    ++divisor_count;
                    if (divisor_count > threshold) break;
                }
            }

            if (divisor_count <= threshold) {   /* acceptable candidate */
                /* append to sequence */
                if (seq_len == seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                    if (!seq) exit(EXIT_FAILURE);
                }
                seq[seq_len++] = candidate;
                break;                            /* move to next term */
            }
            /* otherwise try next integer */
            ++candidate;
        }
    }

    return seq[n - 1];
}
