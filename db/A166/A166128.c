/*
 * A166128  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: for n ≥ 10: a(n) = 30·∑_{i=1}^{9}a(n−i) − 465·a(n−10) ,
 * with a(0)=1, a(1)=32, a(2)=992, a(3)=30752, a(4)=953312,
 * a(5)=29552672, a(6)=916132832, a(7)=28400117792,
 * a(8)=880403651552, a(9)=27292513198112.
 * For n = 10 the numerator contributes +1: 
 * a(10) = 30·∑_{i=1}^{9}a(10−i) − 465·a(0) + 1.
 */
seq_t a(idx_t n)
{
    /* initial seed values a(0)..a(9) */
    static const seq_t init[10] = {
        1ULL,                     /* a(0) */
        32ULL,                    /* a(1) */
        992ULL,                   /* a(2) */
        30752ULL,                 /* a(3) */
        953312ULL,                /* a(4) */
        29552672ULL,              /* a(5) */
        916132832ULL,             /* a(6) */
        28400117792ULL,           /* a(7) */
        880403651552ULL,          /* a(8) */
        27292513198112ULL         /* a(9) */
    };

    if (n < 10)
        return init[(idx_t)n];

    /* compute a(10) with the +1 from the numerator */
    seq_t sum = 0;
    for (idx_t i = 1; i < 10; ++i)
        sum += init[i];

    seq_t a10 = 30 * sum - 465 * init[0] + 1;
    if (n == 10)
        return a10;

    /* buffer holds a(n‑10)…a(n‑1) for the recurrence */
    seq_t buf[10];
    buf[0] = init[1];      /* a(1) */
    for (idx_t k = 1; k < 9; ++k)
        buf[k] = init[1 + k];
    buf[8] = init[9];      /* a(9) */
    buf[9] = a10;          /* a(10) */

    seq_t next = 0;
    for (idx_t cur = 11; cur <= n; ++cur) {
        seq_t s = 0;
        for (idx_t j = 1; j < 10; ++j)
            s += buf[j];         /* a(cur-1)…a(cur-9) */
        next = 30 * s - 465 * buf[0];

        /* shift the buffer left */
        for (idx_t j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = next;
    }
    return next;
}
