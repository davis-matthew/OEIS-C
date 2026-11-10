/*
 * A091851  a(1) = 1; then primes associated with A091850.
 * https://oeis.org/A091851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = Sum_{k=1..n} k*b(k) for n>1, where b = A091850
 *            (b(1)=1, b(n) = least unused positive integer making the sum
 *            prime); computed by search
 * Range:     u8 n <= 8, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static seq_t *A091851_sums = NULL;     /* running sums, index k-1 */
static size_t A091851_len = 0, A091851_cap = 0;
static unsigned char *A091851_used = NULL;
static size_t A091851_ucap = 0;
static size_t A091851_first = 1;       /* smallest possibly unused value */

static void A091851_use_grow(size_t need)
{
    if (need < A091851_ucap) return;
    size_t nc = A091851_ucap ? A091851_ucap : 256;
    while (nc <= need) nc <<= 1;
    A091851_used = mem_resize(unsigned char, A091851_used, nc);
    for (size_t i = A091851_ucap; i < nc; i++) A091851_used[i] = 0;
    A091851_ucap = nc;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A091851_len == 0) {
        A091851_cap = 64;
        A091851_sums = mem_new(seq_t, A091851_cap);
        A091851_use_grow(1);
        A091851_used[1] = 1;
        A091851_sums[0] = (seq_t)1;
        A091851_len = 1;
    }
    while ((idx_t)A091851_len < n) {
        size_t idx = A091851_len + 1;
        seq_t sum = A091851_sums[A091851_len - 1];
        while (A091851_first < A091851_ucap && A091851_used[A091851_first]) A091851_first++;
        size_t c = A091851_first;
        for (;; c++) {
            A091851_use_grow(c);
            if (A091851_used[c]) continue;
            seq_t ns = sum + (seq_t)idx * (seq_t)c;
            if (is_prime_u64((uint64_t)ns)) {
                if (A091851_len >= A091851_cap) {
                    A091851_cap <<= 1;
                    A091851_sums = mem_resize(seq_t, A091851_sums, A091851_cap);
                }
                A091851_sums[A091851_len++] = ns;
                A091851_used[c] = 1;
                break;
            }
        }
    }
    return A091851_sums[(size_t)(n - 1)];
}
