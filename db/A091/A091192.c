/*
 * A091192  Abundant numbers having at least one abundant proper divisor.
 * https://oeis.org/A091192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest abundant number whose at least one proper
 *            divisor is also abundant. The sequence is computed by sieving
 *            abundant numbers and testing each for an abundant proper divisor
 * Range:     u8 none, u16 n <= 1026, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Parameters (can be tuned). */
#define INITIAL_LIMIT 20000U          /* initial sieve size */
#define INITIAL_CAP   2000U           /* initial result vector capacity */

/* Static storage for the sieve and result list. */
static uint64_t sieve_limit = 0;
static uint32_t *spf = NULL;          /* smallest prime factor for each n */
static unsigned char *abun = NULL;    /* abundance flag (0/1) */
static seq_t *res = NULL;             /* result vector */
static size_t res_size = 0;
static size_t res_cap = 0;
static uint64_t checked_up_to = 0;    /* last number for which abun is known */

/* ------------------------------------------------------------------ */
/* Utility: rebuild sieve up to limit.  All arrays are allocated
   anew; the cost is acceptable for the required ranges.               */
static void build_sieve(uint64_t limit) {
    /* Allocate new arrays. */
    uint32_t *new_spf = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    unsigned char *new_abun = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    if (!new_spf || !new_abun) {
        /* Memory error – abort. */
        abort();
    }

    /* Sieve of Eratosthenes to fill spf. */
    for (uint64_t i = 0; i <= limit; ++i) new_spf[i] = 0;
    new_spf[0] = 0;
    new_spf[1] = 1;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {            /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i) {
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
                }
            }
        }
    }

    /* Compute abundance for all numbers up to limit. */
    for (uint64_t num = 1; num <= limit; ++num) {
        /* Compute sigma(num) by prime factorisation. */
        seq_t sigma = 1;
        uint64_t temp = num;
        while (temp > 1) {
            uint32_t p = new_spf[temp];
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t powp = 1;
            for (uint64_t k = 0; k < exp + 1; ++k) powp *= (seq_t)p;
            seq_t term = (powp - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
        new_abun[num] = (sigma > ((seq_t)2 * (seq_t)num)) ? 1 : 0;
    }

    /* Replace old arrays. */
    free(spf);
    free(abun);
    spf = new_spf;
    abun = new_abun;
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that all numbers up to 'limit' have been processed for
   abundance and that 'res' contains all qualifying numbers up to
   the newest limit.  The vector 'res' grows only when it becomes full. */
static void ensure_processed(uint64_t limit) {
    if (limit <= checked_up_to) return;    /* already processed */

    /* Rebuild sieve if necessary. */
    if (limit > sieve_limit) {
        build_sieve(limit);
    }

    /* Process new numbers. */
    for (uint64_t num = checked_up_to + 1; num <= limit; ++num) {
        if (abun[num] == 0) continue;      /* not abundant */

        /* Check for an abundant proper divisor. */
        bool has_abundant = false;
        for (uint64_t d = 2; d * d <= num; ++d) {
            if (num % d == 0) {
                uint64_t d1 = d;
                uint64_t d2 = num / d;
                if (d1 < num && abun[d1]) {
                    has_abundant = true;
                    break;
                }
                if (d2 < num && abun[d2]) {
                    has_abundant = true;
                    break;
                }
            }
        }
        if (has_abundant) {
            /* Append to result vector. */
            if (res_size == res_cap) {
                size_t new_cap = res_cap ? res_cap * 2 : INITIAL_CAP;
                seq_t *tmp = (seq_t*)realloc(res, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                res = tmp;
                res_cap = new_cap;
            }
            res[res_size++] = (seq_t)num;
        }
    }
    checked_up_to = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that the result vector contains at least 'n' terms,
   expanding the sieve limit as necessary.  The expansion strategy
   doubles the current limit until enough terms are found. */
static void ensure_n_terms(idx_t n) {
    while ((idx_t)res_size < n) {
        uint64_t new_limit = sieve_limit ? sieve_limit * 2 : INITIAL_LIMIT;
        ensure_processed(new_limit);
    }
}

/* ------------------------------------------------------------------ */
/* Public function for the OEIS entry. */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n=0 – safety */

    /* Initialise once. */
    if (sieve_limit == 0) {
        build_sieve(INITIAL_LIMIT);
        res = (seq_t*)malloc(INITIAL_CAP * sizeof(seq_t));
        if (!res) abort();
        res_cap = INITIAL_CAP;
        res_size = 0;
        checked_up_to = 0;
    }

    ensure_n_terms(n);
    return res[n - 1];
}
