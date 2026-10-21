/*
 * A074122  Group successively larger composite numbers so that the sum of the
 *          n-th group is a multiple of n. Sequence gives the number of terms
 *          in the n-th group.
 * https://oeis.org/A074122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A074122: Group successively larger composite numbers
   so that the sum of the n-th group is a multiple of n.  The function returns
   the number of terms in the n-th group.  It uses a simple linear scan of
   increasing integers, keeping a list of primes discovered so far to test
   compositeness.  For arbitrary‑precision builds the types seq_t and idx_t
   are opaque, so all arithmetic and modulo operations are performed as
   (seq_t) and (idx_t) conversions. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* global state for incremental computation of the sequence             */
/* --------------------------------------------------------------------- */
static seq_t *group_len = NULL;      /* array of previously computed a(k)   */
static size_t group_cap = 0;         /* current capacity of group_len         */
static size_t group_used = 0;        /* number of groups computed             */

static uint64_t *prime_list = NULL;  /* list of primes found so far           */
static size_t prime_cap = 0;         /* capacity of prime_list                */
static size_t prime_cnt = 0;         /* number of primes stored               */

static uint64_t next_candidate = 4;  /* smallest composite yet to test       */
static idx_t    cur_group     = 1;   /* current group's index (1‑based)      */
static seq_t    cur_sum       = 0;   /* running sum of composites in cur. grp*/
static idx_t    cur_len       = 0;   /* number of composites in cur. grp     */

/* --------------------------------------------------------------------- */
/* utility functions                                                     */
/* --------------------------------------------------------------------- */
static void ensure_prime_cap(size_t need)
{
    if (prime_cap >= need) return;
    size_t newcap = prime_cap ? prime_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, newcap * sizeof(uint64_t));
    prime_list = tmp;
    prime_cap = newcap;
}

static void add_prime(uint64_t p)
{
    ensure_prime_cap(prime_cnt + 1);
    prime_list[prime_cnt++] = p;
}

static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p > x / p) break;
        if (x % p == 0) return false;
    }
    /* x is prime; store it for future tests */
    add_prime(x);
    return true;
}

static void ensure_group_cap(size_t need)
{
    if (group_cap >= need) return;
    size_t newcap = group_cap ? group_cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(group_len, newcap * sizeof(seq_t));
    group_len = tmp;
    group_cap = newcap;
}

static void add_group_length(idx_t len)
{
    ensure_group_cap(group_used + 1);
    group_len[group_used++] = (seq_t)len;
}

/* --------------------------------------------------------------------- */
/* main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* initialise the prime list once */
    if (prime_cnt == 0)
    {
        add_prime(2);
        add_prime(3);
    }

    /* generate groups until we have the n‑th one */
    while (group_used < n)
    {
        /* skip primes */
        while (is_prime_uint64(next_candidate)) next_candidate++;

        /* include this composite number in the current group */
        cur_sum += (seq_t)next_candidate;
        ++cur_len;

        /* if the sum is divisible by the group's index, finish the group */
        if (cur_sum % (seq_t)cur_group == 0)
        {
            add_group_length(cur_len);
            ++cur_group;
            cur_sum = 0;
            cur_len = 0;
        }

        /* advance to the next integer */
        ++next_candidate;
    }

    /* return the length of the requested group */
    return group_len[n - 1];
}
