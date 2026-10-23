/*
 * A039992  Number of distinct primes embedded in prime p(n).
 * https://oeis.org/A039992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct primes that can be formed from * some
 *            or all digits of the n-th prime
 * Range:     u8 n <= 27, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

#define MAX_NUM 1000000   /* enough for primes up to 104729 (6 digits) */

static uint8_t  seen[MAX_NUM];      /* 0/1 flag for numbers seen in this call */
static uint32_t seen_list[MAX_NUM]; /* list of numbers seen, to reset later */
static size_t   seen_list_cnt;

/* Recursive helper: cur = current number built, freq = remaining digit counts,
 * started = whether cur is non-zero (to avoid leading zeros),
 * count = accumulator for distinct primes found.
 */
static void dfs(seq_t cur, int *freq, bool started,
                seq_t *count, size_t *list_cnt)
{
    for (int d = 0; d < 10; ++d) {
        if (!freq[d]) continue;
        if (!started && d == 0) continue; /* avoid leading zeros */

        freq[d]--;                                 /* use digit d */
        seq_t new_val = cur * 10 + d;              /* new number */

        if (new_val < MAX_NUM) {
            size_t idx = (size_t)new_val;
            if (!seen[idx]) {
                seen[idx] = 1;
                seen_list[(*list_cnt)++] = (uint32_t)new_val;
                if (isPrimeGeneric(new_val)) (*count)++;
            }
        }

        dfs(new_val, freq, true, count, list_cnt);
        freq[d]++;                                 /* backtrack */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                           /* undefined for n<1 */

    uint32_t p = get_prime(n);                     /* n-th prime */
    int digit_cnt[10] = {0};

    /* Count digits of p */
    uint32_t tmp = p;
    do {
        digit_cnt[tmp % 10]++;
        tmp /= 10;
    } while (tmp);

    seq_t primes_found = 0;
    seen_list_cnt = 0;

    dfs(0, digit_cnt, false, &primes_found, &seen_list_cnt);

    /* Reset seen flags for numbers generated this call */
    for (size_t i = 0; i < seen_list_cnt; ++i) {
        seen[seen_list[i]] = 0;
    }

    return primes_found;
}
