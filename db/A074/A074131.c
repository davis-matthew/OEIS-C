/*
 * A074131  Group successively larger prime numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives the group sum
 *          divided by n for the n-th group.
 * https://oeis.org/A074131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 27, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (sum of the primes in the n‑th group)/n
 *   The n‑th group contains the smallest run of successive primes
 *   (continuing where the previous group left off) whose sum is a
 *   multiple of n.  The returned value is that sum divided by n.
 *   The sequence is computed by scanning successive primes until the
 *   divisibility condition is satisfied.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static data structures ---------- */

static uint32_t *prime_vec   = NULL; /* list of primes discovered so far    */
static size_t   prime_cap   = 0;     /* allocated size of prime_vec         */
static size_t   prime_cnt   = 0;     /* number of primes stored             */
static uint32_t last_candidate = 3;  /* last prime examined when searching   */

static seq_t *group_val   = NULL;    /* a(n) values computed so far         */
static size_t group_cap   = 0;        /* allocated size of group_val         */
static size_t group_cnt   = 0;        /* number of groups computed           */

static size_t next_prime_pos = 0;    /* next prime in prime_vec to use     */

/* ---------- helper functions ---------- */

/* ensure capacity for prime_vec */
static void expand_prime_vec(void)
{
    if (prime_cnt >= prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_vec = (uint32_t *)realloc(prime_vec,
                                        prime_cap * sizeof(uint32_t));
    }
}

/* add a prime to prime_vec */
static void push_prime(uint32_t p)
{
    expand_prime_vec();
    prime_vec[prime_cnt++] = p;
}

/* generate the next prime after last_candidate */
static void generate_next_prime(void)
{
    uint32_t cand = last_candidate + 1;
    while (1) {
        if (cand % 2 == 0) { cand++; continue; }
        bool is_prime = true;
        for (size_t k = 0; k < prime_cnt; ++k) {
            uint32_t p = prime_vec[k];
            if ((uint64_t)p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            push_prime(cand);
            last_candidate = cand;
            return;
        }
        cand++;
    }
}

/* ensure that prime_vec has at least idx+1 elements */
static void ensure_prime_at(size_t idx)
{
    while (prime_cnt <= idx) generate_next_prime();
}

/* ensure capacity for group_val */
static void expand_group_vec(void)
{
    if (group_cnt >= group_cap) {
        group_cap = group_cap ? group_cap * 2 : 16;
        group_val = (seq_t *)realloc(group_val,
                                     group_cap * sizeof(seq_t));
    }
}

/* compute groups up to at least n (1‑based) */
static void compute_up_to(idx_t n)
{
    while (group_cnt < n) {
        idx_t group_idx = (idx_t)(group_cnt + 1); /* 1‑based */
        seq_t sum = 0;

        while (1) {
            if (next_prime_pos == prime_cnt) generate_next_prime();
            sum += (seq_t)prime_vec[next_prime_pos++];
            if (sum % (seq_t)group_idx == 0) break;
        }
        expand_group_vec();
        group_val[group_cnt++] = sum / (seq_t)group_idx;
    }
}

/* initialise prime list if empty */
static void init_primes(void)
{
    if (prime_cnt == 0) {
        push_prime(2);
        push_prime(3);
        last_candidate = 3;
    }
}

/* ---------- exported function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for zero, return 0 */
    init_primes();
    compute_up_to(n);
    return group_val[n - 1];
}
