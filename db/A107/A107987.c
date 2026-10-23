/*
 * A107987  Odd composite numbers of the form p+2 where p is prime.
 * https://oeis.org/A107987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth odd composite number of the form p+2 with p prime
 * Range:     u8 n <= 36, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation builds a dynamic prime sieve, keeps a list of
   primes and the sequence terms, and extends the sieve when more
   terms are needed. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* static storage for sieve and sequence */
static unsigned char *isPrimeBool = NULL;   /* 1 = prime, 0 = composite */
static idx_t sieve_limit = 0;

static uint32_t *prime_list = NULL;
static idx_t prime_cnt = 0;

static seq_t *term_list = NULL;   /* sequence terms a(1), a(2), ... */
static idx_t term_cnt = 0;

/* dynamic array resizing helpers */
static void ensure_prime_cap(idx_t need) {
    if (prime_cnt == 0 || prime_cnt < need) {
        idx_t new_cap = (prime_cnt == 0) ? 16 : prime_cnt * 2;
        if (new_cap < need) new_cap = need;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        prime_list = tmp;
    }
}

static void push_prime(uint32_t p) {
    if (prime_cnt == 0 || prime_cnt >= prime_cnt) {
        /* just ensure enough capacity, already handled by caller */
        /* no action needed here */
    }
    prime_list[prime_cnt++] = p;
}

static void ensure_term_cap(idx_t need) {
    if (term_cnt == 0 || term_cnt < need) {
        idx_t new_cap = (term_cnt == 0) ? 256 : term_cnt * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(term_list, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        term_list = tmp;
    }
}

/* build or rebuild the prime sieve up to 'limit' */
static void build_sieve(idx_t limit) {
    if (limit == 0) return;
    /* allocate/resize boolean array */
    unsigned char *tmp = (unsigned char *)realloc(isPrimeBool, (size_t)(limit + 1));
    if (!tmp) exit(1);
    isPrimeBool = tmp;
    for (idx_t i = 0; i <= limit; ++i) isPrimeBool[i] = 1;
    if (limit >= 0) isPrimeBool[0] = 0;
    if (limit >= 1) isPrimeBool[1] = 0;
    idx_t sq = (idx_t)sqrt((long double)limit);
    for (idx_t i = 2; i <= sq; ++i) {
        if (isPrimeBool[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                isPrimeBool[j] = 0;
            }
        }
    }
    /* rebuild prime list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (isPrimeBool[i]) {
            /* enlarge if necessary */
            if (prime_cnt >= prime_cnt) ensure_prime_cap(prime_cnt + 1);
            push_prime((uint32_t)i);
        }
    }
    sieve_limit = limit;
}

/* ensure that we have at least 'n' terms precomputed */
static void ensure_terms(idx_t n) {
    /* auto-initialize if needed */
    if (sieve_limit == 0) build_sieve(1000);

    static idx_t prime_pos = 0;          /* next prime index to process */
    while (term_cnt < n) {
        /* if we ran out of primes, extend sieve */
        if (prime_pos >= prime_cnt) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit == 0) new_limit = 1000;
            build_sieve(new_limit);
            continue;
        }

        /* process primes until we need to extend */
        while (prime_pos < prime_cnt && term_cnt < n) {
            uint32_t p = prime_list[prime_pos];
            if (p < 3) {
                ++prime_pos;
                continue;          /* skip p = 2 (gives even 4) */
            }
            idx_t val = (idx_t)p + 2;
            if (val > sieve_limit) break;   /* need more primes: extend */
            /* check odd composite */
            if ((val & 1) && val > 1 && !isPrimeBool[val]) {
                ensure_term_cap(term_cnt + 1);
                term_list[term_cnt++] = (seq_t)val;
            }
            ++prime_pos;
        }

        /* if still not enough terms, extend sieve */
        if (term_cnt < n) {
            idx_t new_limit = sieve_limit * 2;
            if (new_limit == 0) new_limit = 1000;
            build_sieve(new_limit);
            /* prime_pos remains the same index; primes list rebuilt but same order */
        }
    }
}

/* public function */
seq_t a(idx_t n) {
    if (n == 0) { /* behavior below offset is irrelevant; return 0 */
        return (seq_t)0;
    }
    ensure_terms(n);
    return term_list[n - 1];
}
