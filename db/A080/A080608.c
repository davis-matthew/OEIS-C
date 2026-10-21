/*
 * A080608  Deletable primes: primes such that removing some digit leaves
 *          either the empty string or another deletable prime (possibly
 *          preceded by some zeros).
 * https://oeis.org/A080608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 3275, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Deletable primes A080608: a(n) is the n‑th prime such that
   there exists a chain of single‑digit deletions each producing
   a prime, ending at a one‑digit prime.  Leading zeros after a
   deletion are ignored. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT 20000000ULL

static bool *isPrime = NULL;     /* sieve array */
static int8_t *delMemo = NULL;   /* -1 unknown, 0 false, 1 true */
static bool initialized = false;

static void init(void)
{
    if (initialized) return;
    size_t lim = (size_t)LIMIT;

    isPrime = (bool *)malloc((lim + 1) * sizeof(bool));
    delMemo = (int8_t *)malloc((lim + 1) * sizeof(int8_t));
    memset(isPrime, 1, (lim + 1) * sizeof(bool));
    isPrime[0] = isPrime[1] = false;

    for (size_t i = 2; i * i <= lim; ++i) {
        if (isPrime[i]) {
            for (size_t j = i * i; j <= lim; j += i) {
                isPrime[j] = false;
            }
        }
    }

    for (size_t i = 0; i <= lim; ++i) delMemo[i] = -1;
    delMemo[0] = 0; delMemo[1] = 0;

    initialized = true;
}

static bool isDelRec(uint64_t n)
{
    int8_t m = delMemo[n];
    if (m != -1) return m;

    bool res;
    if (n < 10) {
        res = isPrime[n];
        delMemo[n] = res;
        return res;
    }
    if (!isPrime[n]) {
        res = false;
        delMemo[n] = res;
        return res;
    }

    /* digits in most‑significant‑first order */
    uint8_t tmp[20];
    int len = 0;
    uint64_t t = n;
    while (t) { tmp[len++] = t % 10; t /= 10; }
    for (int i = 0; i < len / 2; ++i) {
        uint8_t u = tmp[i]; tmp[i] = tmp[len - 1 - i]; tmp[len - 1 - i] = u;
    }

    for (int i = 0; i < len; ++i) {
        uint64_t q = 0;
        for (int j = 0; j < len; ++j) {
            if (j == i) continue;
            q = q * 10 + tmp[j];
        }
        if (q <= LIMIT && isPrime[q] && isDelRec(q)) {
            res = true;
            delMemo[n] = res;
            return res;
        }
    }
    res = false;
    delMemo[n] = res;
    return res;
}

seq_t a(idx_t n)
{
    init();
    uint64_t target = n;
    uint64_t count = 0;
    for (uint64_t p = 2; p <= LIMIT; ++p) {
        if (!isPrime[p]) continue;
        if (isDelRec(p)) {
            ++count;
            if (count == target) return (seq_t)p;
        }
    }
    /* Fallback: should not be reached for n ≤ 10000 with LIMIT = 20 M */
    return (seq_t)0;
}
