/*
 * A109411  Partition the sequence of positive integers into minimal groups so
 *          that sum of terms in each group is a semiprime; sequence gives
 *          sizes of the groups.
 * https://oeis.org/A109411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9447, u16 n <= 9447, u32 n <= 9447, u64 n <= 9447,
 *            u128 n <= 9447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A109411: minimal groups of consecutive positive integers whose
   sums are semiprimes.  a(n) is the size of the n‑th group.
   The function stores computed groups in a dynamic array and
   expands it as needed.  A simple trial–division test is used to
   determine whether a number is a semiprime. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- helper: test for semiprime --------------------------------- */
static bool is_semiprime_uint64(uint64_t n)
{
    if (n < 4)          /* smallest semiprime is 2*2 = 4 */
        return false;

    uint64_t cnt = 0;
    for (uint64_t p = 2; p * p <= n && cnt <= 2; ++p) {
        while (n % p == 0) {
            ++cnt;
            n /= p;
            if (cnt > 2) return false;  /* early exit */
        }
    }
    if (n > 1) ++cnt;         /* remaining prime factor, if any */
    return cnt == 2;
}

/* ---- state kept between calls ----------------------------------- */
static seq_t *group_sizes = NULL;   /* stored a(1),a(2),… */
static idx_t  group_len   = 0;      /* number of groups computed */
static idx_t  group_cap   = 0;      /* array capacity */
static idx_t  next_start  = 1;      /* first integer of the next group */

/* ensure capacity for at least 'need' elements */
static void grow_array(idx_t need)
{
    if (group_cap >= need) return;
    idx_t new_cap = group_cap ? group_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(group_sizes, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* out of memory */
    group_sizes = tmp;
    group_cap   = new_cap;
}

/* find the minimal end of the group starting at 'a' */
static idx_t find_group_end(idx_t a)
{
    for (idx_t b = a; ; ++b) {
        /* sum of arithmetic progression a..b:
           (a + b) * (b - a + 1) / 2  */
        __uint128_t sum128 = (__uint128_t)(a + b) * (b - a + 1) / 2;
        uint64_t sum = (uint64_t)sum128;   /* fits in 64‑bit for all used n */
        if (is_semiprime_uint64(sum))
            return b;
    }
}

/* ---- the required function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0 and below */

    /* extend the table until it contains the n‑th group */
    while (group_len < n) {
        idx_t end   = find_group_end(next_start);
        idx_t size  = end - next_start + 1;
        grow_array(group_len + 1);
        group_sizes[group_len] = (seq_t)size;
        ++group_len;
        next_start = end + 1;
    }

    return group_sizes[n - 1];
}
