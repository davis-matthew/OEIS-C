/*
 * A129943  Left truncatable primes in base 7 (written in decimal form).
 * https://oeis.org/A129943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   generate the left‑truncatable primes in base 7 by breadth‑first
 *            search over base‑7 digit strings. Starting from the single‑digit
 *            base‑7 primes {2,3,5}, prepend each possible digit (1–6) and
 *            keep the resulting number if it is still prime (is_prime_big).
 *            All discovered numbers are sorted in ascendi...
 * Range:     u8 none, u16 n <= 20, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: full, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

struct Node {
    seq_t val;   /* decimal value of the base‑7 number */
    seq_t pow7;  /* current power of 7, i.e. 7^digits_of_val */
};

static seq_t *seq_cached = NULL;
static idx_t  seq_len    = 0;
static bool   generated  = false;

static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

static void generate_sequence(void)
{
    /* queue for BFS: dynamic array of Node */
    size_t q_cap = 8, q_len = 0;
    struct Node *queue = mem_new(struct Node, q_cap);

    /* result array: dynamic array of seq_t */
    size_t res_cap = 16, res_len = 0;
    seq_t *results = mem_new(seq_t, res_cap);

    /* initial single‑digit primes in base 7 */
    const uint8_t start_digits[] = {2, 3, 5};
    for (size_t i = 0; i < sizeof(start_digits) / sizeof(start_digits[0]); ++i)
    {
        /* enqueue new node */
        if (q_len == q_cap)
        {
            q_cap <<= 1;
            queue = mem_resize(struct Node, queue, q_cap);
        }
        queue[q_len++] = (struct Node){ (seq_t)start_digits[i], (seq_t)7 };

        /* add to results */
        if (res_len == res_cap)
        {
            res_cap <<= 1;
            results = mem_resize(seq_t, results, res_cap);
        }
        results[res_len++] = (seq_t)start_digits[i];
    }

    /* BFS expansion */
    size_t head = 0;
    while (head < q_len)
    {
        struct Node cur = queue[head++];
        for (uint8_t d = 1; d <= 6; ++d)
        {
            seq_t new_val = (seq_t)d * cur.pow7 + cur.val;
            seq_t new_pow = cur.pow7 * (seq_t)7;

            if (is_prime_big(new_val))
            {
                /* store result */
                if (res_len == res_cap)
                {
                    res_cap <<= 1;
                    results = mem_resize(seq_t, results, res_cap);
                }
                results[res_len++] = new_val;

                /* enqueue for further expansion */
                if (q_len == q_cap)
                {
                    q_cap <<= 1;
                    queue = mem_resize(struct Node, queue, q_cap);
                }
                queue[q_len++] = (struct Node){ new_val, new_pow };
            }
        }
    }

    mem_free(queue);

    /* sort results in ascending decimal order */
    mem_qsort(results, res_len, sizeof(seq_t), cmp_seq);

    seq_cached = results;
    seq_len    = res_len;
}

/* external linkage: returns the n‑th term (1‑based) of A129943 */
seq_t a(idx_t n)
{
    if (!generated)
    {
        generate_sequence();
        generated = true;
    }

    if (n == (idx_t)0 || n > seq_len)
        return (seq_t)0;

    return seq_cached[n - 1];
}
