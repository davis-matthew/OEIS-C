/*
 * A152626  Decimal digits of amicable pairs (x,y) with x > y, ordered by x.
 * https://oeis.org/A152626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digits of amicable pairs (x > y), ordered by increasing x.
   The nth term a(n) is the n‑th decimal digit of the concatenation of
   all such pairs.  */

/* required types are supplied by the harness */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* -------------------------------------------------------------------------- */
/* Static state:                                      */
/*   digit_buffer   – concatenated digits of all found pairs       */
/*   digit_buffer_len – number of digits stored                    */
/*   current_limit – upper bound used for divisor‑sum calculation */
/* -------------------------------------------------------------------------- */
static seq_t *digit_buffer = NULL;
static size_t digit_buffer_len = 0;
static uint64_t current_limit = 0;  /* starts at 0, set during first call */

/* -------------------------------------------------------------------------- */
/* Helper: count decimal digits of a 64‑bit integer                         */
static size_t digits_of_uint64(uint64_t n)
{
    size_t d = 0;
    do { ++d; n /= 10; } while (n);
    return d;
}

/* -------------------------------------------------------------------------- */
/* Helper: append the decimal digits of n to buf at position pos          */
static void append_digits(seq_t *buf, size_t *pos, uint64_t n)
{
    /* collect digits in reverse order */
    uint8_t rev[32];
    size_t l = 0;
    do { rev[l++] = (uint8_t)(n % 10); n /= 10; } while (n);
    /* write them forward into the buffer */
    for (size_t i = 0; i < l; ++i)
        buf[(*pos)++] = (seq_t)rev[l - 1 - i];
}

/* -------------------------------------------------------------------------- */
/* Helper: structure for a pair (x, y)                                      */
struct pair { uint64_t x; uint64_t y; };

/* Comparator for qsort: order by x ascending                           */
static int pair_cmp(const void *a, const void *b)
{
    const struct pair *pa = (const struct pair *)a;
    const struct pair *pb = (const struct pair *)b;
    if (pa->x < pb->x) return -1;
    if (pa->x > pb->x) return 1;
    return 0;
}

/* -------------------------------------------------------------------------- */
/* Ensure digit_buffer contains at least n digits.                        */
static void ensure_digits(idx_t n)
{
    /* n is 1‑based; convert to 0‑based size_t for comparison */
    size_t need = (size_t)n;

    if (need <= digit_buffer_len) return;          /* nothing to do */

    /* first time: pick a starting limit */
    if (current_limit == 0) current_limit = 1000000;

    /* grow until we have enough digits */
    while (digit_buffer_len < need)
    {
        /* ------------------------------------------------------------------ */
        /* 1. Compute sum of proper divisors for all numbers up to current_limit */
        /* ------------------------------------------------------------------ */
        uint64_t *sum = (uint64_t *)calloc(current_limit + 1, sizeof(uint64_t));
        if (!sum) { /* allocation failed – abort gracefully */
            free(digit_buffer);
            digit_buffer = NULL;
            digit_buffer_len = 0;
            return;
        }
        for (uint64_t i = 1; i <= current_limit / 2; ++i)
            for (uint64_t j = i * 2; j <= current_limit; j += i)
                sum[j] += i;

        /* ------------------------------------------------------------------ */
        /* 2. Detect amicable pairs (x > y) within current_limit               */
        /* ------------------------------------------------------------------ */
        struct pair *pairs = NULL;
        size_t pair_capacity = 0;
        size_t pair_count = 0;

        for (uint64_t y = 2; y <= current_limit; ++y)
        {
            uint64_t x = sum[y];
            if (x > y && x <= current_limit && sum[x] == y)
            {
                if (pair_count == pair_capacity)
                {
                    size_t new_cap = pair_capacity ? pair_capacity * 2 : 256;
                    struct pair *tmp = (struct pair *)realloc(pairs, new_cap * sizeof(struct pair));
                    if (!tmp) { /* realloc failed – clean up */
                        free(sum);
                        free(pairs);
                        free(digit_buffer);
                        digit_buffer = NULL;
                        digit_buffer_len = 0;
                        return;
                    }
                    pairs = tmp;
                    pair_capacity = new_cap;
                }
                pairs[pair_count].x = x;
                pairs[pair_count].y = y;
                ++pair_count;
            }
        }

        /* ------------------------------------------------------------------ */
        /* 3. Sort pairs by x (the larger member)                               */
        /* ------------------------------------------------------------------ */
        qsort(pairs, pair_count, sizeof(struct pair), pair_cmp);

        /* ------------------------------------------------------------------ */
        /* 4. Compute total number of digits to be produced in this round         */
        /* ------------------------------------------------------------------ */
        size_t total_digits = 0;
        for (size_t i = 0; i < pair_count; ++i)
        {
            total_digits += digits_of_uint64(pairs[i].x);
            total_digits += digits_of_uint64(pairs[i].y);
        }

        /* ------------------------------------------------------------------ */
        /* 5. Build the new digit buffer (overwrites old buffer if any)          */
        /* ------------------------------------------------------------------ */
        seq_t *newbuf = (seq_t *)calloc(total_digits, sizeof(seq_t));
        if (!newbuf) { /* allocation failed – clean up */
            free(sum);
            free(pairs);
            free(digit_buffer);
            digit_buffer = NULL;
            digit_buffer_len = 0;
            return;
        }
        size_t pos = 0;
        for (size_t i = 0; i < pair_count; ++i)
        {
            append_digits(newbuf, &pos, pairs[i].x);
            append_digits(newbuf, &pos, pairs[i].y);
        }

        /* ------------------------------------------------------------------ */
        /* 6. Swap buffers and free temporary structures                       */
        /* ------------------------------------------------------------------ */
        free(digit_buffer);
        free(sum);
        free(pairs);

        digit_buffer = newbuf;
        digit_buffer_len = total_digits;

        /* ------------------------------------------------------------------ */
        /* 7. If still not enough, increase the sieve limit and loop again        */
        /* ------------------------------------------------------------------ */
        if (digit_buffer_len < need)
            current_limit *= 2;
    }
}

/* -------------------------------------------------------------------------- */
/* Public function required by the harness.                                 */
seq_t a(idx_t n)
{
    ensure_digits(n);
    /* a(1) corresponds to digit_buffer[0] because the input index is 1‑based */
    return digit_buffer[(size_t)n - 1];
}
