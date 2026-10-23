/*
 * A100716  Numbers k such that p^p divides k for some prime p.
 * https://oeis.org/A100716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A100716: numbers k such that p^p divides k for some prime p */

#include <stdlib.h>
#include <stdbool.h>

/* cached sequence terms */
static seq_t  *seq_buf = NULL;
static idx_t   seq_len = 0;
static idx_t   seq_cap = 0;

/* cached prime powers p^p */
static seq_t  *prime_pw_buf = NULL;
static idx_t   prime_pw_len = 0;
static idx_t   prime_pw_cap = 0;
static idx_t   last_prime      = 1;      /* last prime already generated */
static idx_t   nextprime_cand  = 2;      /* candidate number for next prime */

/* current number being tested for inclusion */
static seq_t   next_candidate = 2;        /* start testing from 2 */

/* generate the next prime and store its power p^p */
static void generate_next_prime(void)
{
    for (;;) {
        bool isprime = true;
        for (idx_t i = 2; i * i <= nextprime_cand; ++i) {
            if (nextprime_cand % i == 0) { isprime = false; break; }
        }
        if (isprime) {
            last_prime = nextprime_cand;
            /* compute p^p */
            seq_t power = 1;
            for (idx_t i = 0; i < last_prime; ++i) power *= last_prime;

            /* enlarge prime power array if necessary */
            if (prime_pw_len == prime_pw_cap) {
                prime_pw_cap = prime_pw_cap ? prime_pw_cap * 2 : 16;
                prime_pw_buf = (seq_t *)realloc(prime_pw_buf,
                                                prime_pw_cap * sizeof(seq_t));
                if (!prime_pw_buf) abort();
            }
            prime_pw_buf[prime_pw_len++] = power;
            ++nextprime_cand;
            return;
        }
        ++nextprime_cand;
    }
}

/* ensure that at least n terms are cached */
static void ensure_cached(idx_t n)
{
    /* generate prime powers as needed up to current candidate */
    while (prime_pw_len == 0 || last_prime < next_candidate) {
        generate_next_prime();
    }

    while (seq_len < n) {
        bool divisible = false;
        for (idx_t i = 0; i < prime_pw_len; ++i) {
            if (next_candidate % prime_pw_buf[i] == 0) {
                divisible = true;
                break;
            }
        }
        if (divisible) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_buf = (seq_t *)realloc(seq_buf,
                                            seq_cap * sizeof(seq_t));
                if (!seq_buf) abort();
            }
            seq_buf[seq_len++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* external linkage as required */
seq_t a(idx_t n)
{
    if (n < 1) return 0;              /* undefined for n ≤ 0 */
    ensure_cached(n);
    return seq_buf[n - 1];
}
