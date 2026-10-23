/*
 * A008040  Coordination sequence T3 for Zeolite Code ATS.
 * https://oeis.org/A008040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) =  a(n‑1) – a(n‑2) + a(n‑3) – a(n‑4) + 3a(n‑5) – 2a(n‑6) + 2a(n‑7) – 2a(n‑8)
           + 2a(n‑9) – 3a(n‑10) + a(n‑11) – a(n‑12) + a(n‑13) – a(n‑14) + a(n‑15)  (n ≥ 16) */
seq_t a(idx_t n)
{
    /* first 16 terms (a(0)…a(15)) */
    static const seq_t init[16] = {
        1,   /* a(0) */
        4,   /* a(1) */
        10,  /* a(2) */
        19,  /* a(3) */
        32,  /* a(4) */
        51,  /* a(5) */
        72,  /* a(6) */
        96,  /* a(7) */
        124, /* a(8) */
        155, /* a(9) */
        196, /* a(10) */
        238, /* a(11) */
        278, /* a(12) */
        324, /* a(13) */
        374, /* a(14) */
        436  /* a(15) */
    };

    if (n < 16)
        return init[n];

    /* buffer holds a(i‑1)…a(i‑15) for the current i */
    seq_t buf[15];
    buf[0] = init[15]; /* a(n-1) when i=16 */
    buf[1] = init[14];
    buf[2] = init[13];
    buf[3] = init[12];
    buf[4] = init[11];
    buf[5] = init[10];
    buf[6] = init[9];
    buf[7] = init[8];
    buf[8] = init[7];
    buf[9] = init[6];
    buf[10] = init[5];
    buf[11] = init[4];
    buf[12] = init[3];
    buf[13] = init[2];
    buf[14] = init[1];

    for (idx_t i = 16; i <= n; ++i) {
        /* compute next term using the recurrence */
        seq_t pos = buf[0]                            /* a(n-1) */
                 + buf[2]                            /* a(n-3) */
                 + (buf[4] * 3)                      /* 3 * a(n-5) */
                 + (buf[6] * 2)                      /* 2 * a(n-7) */
                 + (buf[8] * 2)                      /* 2 * a(n-9) */
                 + buf[10]                           /* a(n-11) */
                 + buf[12]                           /* a(n-13) */
                 + buf[14];                          /* a(n-15) */

        seq_t neg = buf[1]                            /* a(n-2) */
                 + buf[3]                            /* a(n-4) */
                 + (buf[5] * 2)                      /* 2 * a(n-6) */
                 + (buf[7] * 2)                      /* 2 * a(n-8) */
                 + (buf[9] * 3)                      /* 3 * a(n-10) */
                 + buf[11]                           /* a(n-12) */
                 + buf[13];                          /* a(n-14) */

        seq_t next = pos - neg;

        /* Shift buffer to slide window: new a(n) becomes buf[0] */
        for (int j = 14; j > 0; --j)
            buf[j] = buf[j - 1];
        buf[0] = next;
    }

    return buf[0];
}
