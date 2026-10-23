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
 * Range:     u8 none, u16 n <= 20, u32 n <= 20, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

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
    struct Node *queue = (struct Node *)malloc(q_cap * sizeof(struct Node));

    /* result array: dynamic array of seq_t */
    size_t res_cap = 16, res_len = 0;
    seq_t *results = (seq_t *)malloc(res_cap * sizeof(seq_t));

    /* initial single‑digit primes in base 7 */
    const uint8_t start_digits[] = {2, 3, 5};
    for (size_t i = 0; i < sizeof(start_digits) / sizeof(start_digits[0]); ++i)
    {
        /* enqueue new node */
        if (q_len == q_cap)
        {
            q_cap <<= 1;
            queue = (struct Node *)realloc(queue,
                                            q_cap * sizeof(struct Node));
        }
        queue[q_len++] = (struct Node){ (seq_t)start_digits[i], (seq_t)7 };

        /* add to results */
        if (res_len == res_cap)
        {
            res_cap <<= 1;
            results = (seq_t *)realloc(results,
                                        res_cap * sizeof(seq_t));
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
                    results = (seq_t *)realloc(results,
                                                res_cap * sizeof(seq_t));
                }
                results[res_len++] = new_val;

                /* enqueue for further expansion */
                if (q_len == q_cap)
                {
                    q_cap <<= 1;
                    queue = (struct Node *)realloc(queue,
                                                   q_cap * sizeof(struct Node));
                }
                queue[q_len++] = (struct Node){ new_val, new_pow };
            }
        }
    }

    free(queue);

    /* sort results in ascending decimal order */
    qsort(results, res_len, sizeof(seq_t), cmp_seq);

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
