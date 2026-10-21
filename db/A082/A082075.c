/*
 * A082075  First differences of primes of the form 4*k+3 (A002145).
 * https://oeis.org/A082075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A082075: differences of consecutive primes ≡ 3 (mod 4)
    a(n) = A002145(n+1) - A002145(n)   (offset 1)   */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are supplied by the build  */
static idx_t *prime_list   = NULL;   /* primes found so far, 2 included   */
static idx_t prime_count   = 0;
static idx_t prime_alloc   = 0;

static seq_t *diff_list    = NULL;   /* a(1)=diff_list[0]                */
static idx_t diff_count    = 0;
static idx_t diff_alloc    = 0;

static idx_t last_candidate = 3;     /* next odd number to test          */
static idx_t last_4k3 = 0;           /* last prime ≡ 3 (mod 4)           */
static bool have_first_4k3 = false;  /* true after first 4k+3 seen      */

/*--- dynamic array helpers -----------------------------------------------------*/
static void grow_idx_array(idx_t **arr, idx_t *cnt, idx_t *alloc, idx_t needed)
{
    if (needed <= *alloc) return;
    idx_t new_alloc = (*alloc == 0) ? 64 : *alloc * 2;
    while (new_alloc < needed) new_alloc <<= 1;
    *arr = (idx_t *)realloc(*arr, new_alloc * sizeof(idx_t));
    if (!*arr) abort();
    *alloc = new_alloc;
}

static void grow_seq_array(seq_t **arr, idx_t *cnt, idx_t *alloc, idx_t needed)
{
    if (needed <= *alloc) return;
    idx_t new_alloc = (*alloc == 0) ? 64 : *alloc * 2;
    while (new_alloc < needed) new_alloc <<= 1;
    *arr = (seq_t *)realloc(*arr, new_alloc * sizeof(seq_t));
    if (!*arr) abort();
    *alloc = new_alloc;
}

/*--- push helpers -----------------------------------------------------------*/
static void push_prime(idx_t p)
{
    grow_idx_array(&prime_list, &prime_count, &prime_alloc, prime_count + 1);
    prime_list[prime_count++] = p;
}

static void push_diff(seq_t d)
{
    grow_seq_array(&diff_list, &diff_count, &diff_alloc, diff_count + 1);
    diff_list[diff_count++] = d;
}

/*--- primality test using the already found primes -------------------------*/
static bool is_prime_candidate(idx_t x)
{
    if (x <= 3) return true;            /* 2 and 3 are prime */
    for (idx_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        idx_t sq = p * p;
        if (sq > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/*--- ensure that diff_list contains at least n elements ---------------------*/
static void ensure_at_least(idx_t n)   /* n = desired number of a(n) values */
{
    /* prime list must contain 2 to start */
    if (prime_count == 0) {
        push_prime(2);
    }

    while (diff_count < n) {
        idx_t p = last_candidate;
        last_candidate += 2;           /* next odd candidate */

        if (is_prime_candidate(p)) {
            push_prime(p);
            if ((p & 3) == 3) {        /* p ≡ 3 (mod 4) */
                if (have_first_4k3) {
                    seq_t d = (seq_t)(p - last_4k3);
                    push_diff(d);
                }
                last_4k3 = p;
                have_first_4k3 = true;
            }
        }
    }
}

/*--- public sequence function -----------------------------------------------*/
seq_t a(idx_t n)                /* n >= 1  -> a(n) */
{
    if (n == 0) return (seq_t)0;  /* undefined offset */
    ensure_at_least(n);
    return diff_list[n - 1];
}
