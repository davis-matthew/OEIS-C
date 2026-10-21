/*
 * A208554  Number of nX7 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence (OEIS A208554):
   a(1)=42, a(2)=1764, a(3)=10710, a(4)=65025, a(5)=221340,
   a(6)=753424, a(7)=1913940, a(8)=4862025, a(9)=10332630,
   a(10)=21958596, a(11)=41363322, a(12)=77915929, a(13)=134523480,
   a(14)=232257600, a(15)=375406920, a(16)=606784689.
   For n≥17:
   a(n)=2*a(n‑1)+6*a(n‑2)-14*a(n‑3)-14*a(n‑4)+42*a(n‑5)+14*a(n‑6)
        -70*a(n‑7)+70*a(n‑9)-14*a(n‑10)-42*a(n‑11)+14*a(n‑12)+14*a(n‑13)
        -6*a(n‑14)-2*a(n‑15)+a(n‑16). */

#include <stdlib.h>

static const seq_t init[16] = {
    (seq_t)42,            /* a(1) */
    (seq_t)1764,          /* a(2) */
    (seq_t)10710,         /* a(3) */
    (seq_t)65025,         /* a(4) */
    (seq_t)221340,        /* a(5) */
    (seq_t)753424,        /* a(6) */
    (seq_t)1913940,       /* a(7) */
    (seq_t)4862025,       /* a(8) */
    (seq_t)10332630,      /* a(9) */
    (seq_t)21958596,      /* a(10) */
    (seq_t)41363322,      /* a(11) */
    (seq_t)77915929,      /* a(12) */
    (seq_t)134523480,     /* a(13) */
    (seq_t)232257600,     /* a(14) */
    (seq_t)375406920,     /* a(15) */
    (seq_t)606784689      /* a(16) */
};

seq_t a(idx_t n)
{
    /* Behaviour for n < 1 is undefined; return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Memoisation array: arr[0] corresponds to a(1). */
    static seq_t *arr = NULL;
    static idx_t used = 0;   /* number of elements currently stored */

    if (n > used) {
        /* Grow the array to hold terms up to n. */
        seq_t *tmp = NULL;
        if (arr == NULL)
            tmp = (seq_t *)malloc((size_t)n * sizeof(seq_t));
        else
            tmp = (seq_t *)realloc(arr, (size_t)n * sizeof(seq_t));
        if (tmp == NULL)  /* allocation failed */
            return (seq_t)0;
        arr = tmp;

        /* Compute new terms from the first missing index up to n. */
        for (idx_t i = used; i < n; ++i) {
            idx_t k = i + 1;   /* current term index a(k) */

            if (k <= 16) {
                arr[i] = init[k - 1];
            } else {
                /* Positive contributions */
                seq_t pos = (seq_t)2 * arr[k - 2]          /* a(k-1) */
                          + (seq_t)6 * arr[k - 3]          /* a(k-2) */
                          + (seq_t)42 * arr[k - 6]         /* a(k-5) */
                          + (seq_t)14 * arr[k - 7]         /* a(k-6) */
                          + (seq_t)70 * arr[k -10]         /* a(k-9) */
                          + (seq_t)14 * arr[k -13]         /* a(k-12) */
                          + (seq_t)14 * arr[k -14]         /* a(k-13) */
                          + arr[k -17];                   /* a(k-16) */

                /* Negative contributions */
                seq_t neg = (seq_t)14 * arr[k - 4]          /* a(k-3) */
                          + (seq_t)14 * arr[k - 5]          /* a(k-4) */
                          + (seq_t)70 * arr[k - 8]          /* a(k-7) */
                          + (seq_t)14 * arr[k -11]          /* a(k-10) */
                          + (seq_t)42 * arr[k -12]          /* a(k-11) */
                          + (seq_t)6  * arr[k -15]          /* a(k-14) */
                          + (seq_t)2  * arr[k -16];         /* a(k-15) */

                arr[i] = pos - neg;
            }
        }
        used = n;
    }

    return arr[n - 1];
}
