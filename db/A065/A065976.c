/*
 * A065976  Consecutive terms of A065966 which are also consecutive integers.
 * https://oeis.org/A065976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k such that both k and k+1 are in A065966 (phi(k)/2
 *            prime); found by a search with memoization
 * Range:     u8 n <= 8, u16 n <= 66, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 57 terms through n = 977 (2026-11-07)
 * Helpers:   is_prime_u64, mem, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/totient_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static bool A065976_in(seq_t k)
{
    seq_t phi = totient_seq(k);
    if ((phi & 1) != 0 || phi < 2) return false;
    seq_t half = phi >> 1;
    return is_prime_u64((uint64_t)half);
}

static seq_t *A065976_seq = NULL;
static size_t A065976_len = 0;
static size_t A065976_cap = 0;
static seq_t A065976_next = 1;      /* next k to test as the lower member */
static bool A065976_have = false;   /* whether A065976_in(A065976_next) is cached */
static bool A065976_cur = false;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A065976_len < (size_t)n)
    {
        if (!A065976_have) {
            A065976_cur = A065976_in(A065976_next);
            A065976_have = true;
        }
        bool nxt = A065976_in(A065976_next + 1);
        if (A065976_cur && nxt) {
            if (A065976_len >= A065976_cap) {
                size_t nc = A065976_cap ? A065976_cap * 2 : 16;
                A065976_seq = mem_resize(seq_t, A065976_seq, nc);
                if (!A065976_seq) exit(1);
                A065976_cap = nc;
            }
            A065976_seq[A065976_len++] = A065976_next;
        }
        A065976_cur = nxt;
        ++A065976_next;
    }
    return A065976_seq[(size_t)n - 1];
}
